/* Copyright (C) 2017 Roman Koshelev */

#ifndef LABYRINTH_MESSAGE_HPP
#define LABYRINTH_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include "promotion.hpp"
#include "codes.hpp"
#include "../game-mechanics/game.h"

namespace headers_t
{
	enum header : std::uint32_t
	{
		authorization,
		registration,
		get_list_rooms,
		list_rooms,
		create_room,
		join_room,
		enter_room,
		escape_from,
		server_log,     //зарезервировано для сервера
		ping,
		play_mes,
		no_answer,

		null_value,
	};
}

template <class... Types>
class message
{

public:
	enum { header_length = 12 };
	enum { max_body_length = 512 };

	message() : body_length_(0),
		header_type_(headers_t::null_value),
		message_type_(0)
	{
	}

	template <class Ty>
	message(Ty line, headers_t::header h)
		: body_length_(binary_size(line)),
		header_type_(h),
		message_type_(type_to_id(identity<Ty>{})) //////////
	{
		data_ = line;
	}

	std::size_t length() const
	{
		return header_length + body_length_;
	}

	template<class Ty>
	Ty get() const
	{
		return boost::get<Ty>(data_);
	}

	std::size_t body_length() const
	{
		return body_length_;
	}

	bool decode_header(char* mem)
	{
		char header[4 + 1];
		header[0] = '\0';
		std::strncat(header, mem, 4);
		body_length_ = std::atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		header[0] = '\0';
		std::strncat(header, mem + 4, 4);
		header_type_ = headers_t::header(std::atoi(header));
		if (header_type_ > headers_t::null_value) {
			return false;
		}
		header[0] = '\0';
		std::strncat(header, mem + 8, 4);
		message_type_ = (std::atoi(header));
		/*if (message_type_ > messages_t::null_value) {
		return false;
		}*/
		return true;
	}


	bool decode_body(char* mem)
	{
		const auto dec_fun = [this, mem](auto Ty) {
			decltype(Ty) buffer;
			decodeX(mem, buffer);
			data_ = buffer;
		};
		promotion<Types...>::exec(message_type_, dec_fun);

		return true;
	}

	void encode_header(char* mem) const
	{
		mem[0] = '\0';
		std::sprintf(mem, "%4u", static_cast<std::uint32_t>(body_length_));
		std::sprintf(mem + 4, "%4u", static_cast<std::uint32_t>(header_type_));
		std::sprintf(mem + 8, "%4u", static_cast<std::uint32_t>(message_type_));
	}

	void encode_body(char* mem)
	{
		boost::apply_visitor([&mem](auto& aut) {  encodeX(mem, aut); }, data_);
	}

	headers_t::header get_package_type() const
	{
		return header_type_;
	}

private:

	boost::variant<Types...> data_;
	std::size_t body_length_;
	headers_t::header header_type_;
	std::uint32_t message_type_;
};

using labyrinth_message = message<Turn, ShowTurn, ShowOtherTurn, Start_room, join_room, get_list_room, list_room_package>;

#endif // LABYRINTH_MESSAGE_HPP
