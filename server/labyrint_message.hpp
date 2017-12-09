/* Copyright (C) 2017 Roman Koshelev */

#ifndef LABYRINTH_MESSAGE_HPP
#define LABYRINTH_MESSAGE_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <type_traits>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include "../game/game.h"

inline void log(const char * s)
{
	std::clog << s << "\n";
}

#define DEBUG_LOGIC

#define DEBUG_LOGIC
#ifdef DEBUG_LOGIC

#define assert_debug(flag,  ...) \
	if ((flag)) fprintf(stderr,  __VA_ARGS__)

#define debug_call(variable, expression)  \
	printf("begin - "#expression" \n");   \
	variable = expression;                \
	printf("end   - "#expression" \n")

#else

#define assert_debug(arg1, ...) ;
#define debug_call(variable, expression) variable = expression

#endif

namespace headers_t
{
	enum header : std::uint32_t
	{
		authorization,
		registration,
		get_list_rooms,
		create_room,
		escape_from,
		server_log,     //зарезервировано для сервера
		ping,
		test_mes,
		no_answer,

		null_value,
	};
}

namespace messages_t
{
	enum message : std::uint32_t
	{
		mes_test,
		ShowTurn_e,
		ShowOtherTurn_e,
		Turn_e,


		null_value,
	};

	template<class Ty>
	message TypeToEnum() 
	{
			if (std::is_same<Ty, ShowTurn>::value) return ShowTurn_e;
			if (std::is_same<Ty, ShowOtherTurn>::value) return ShowOtherTurn_e;
			if (std::is_same<Ty, Turn>::value) return Turn_e;
			assert_debug(true, "ошибка TypeToEnum\n");
	}

}

struct encode : boost::static_visitor<>
{
	void operator() (char* mem, const std::array<char, 512>& data) const
	{
		std::memcpy(mem, data.data(), 512);
	}

	template<class Ty>
	std::enable_if_t<std::is_trivially_copyable<Ty>::value, void> 
	operator() (char* mem, const Ty& data) const
	{
		std::memcpy(mem, (char*)(&data), sizeof(Ty));
	}
};

class labyrinth_message
{

public:
	enum { header_length = 12 };
	enum { max_body_length = 512 };

	labyrinth_message() : body_length_(0),
		header_type_(headers_t::null_value),
		message_type_(messages_t::null_value)
	{
	}

	labyrinth_message(char* line, const headers_t::header package_type, const messages_t::message message_type)
		: body_length_(sizeof(ShowTurn)),
		header_type_(package_type),           // не использовать
		message_type_(message_type)
	{
		std::array<char, max_body_length> buf;
		std::memcpy(buf.data(), line, body_length_);
		data_ = buf;
	}

	template <class Ty>
	labyrinth_message(Ty line)
		: body_length_(sizeof(Ty)),
		header_type_(headers_t::test_mes),           
		message_type_(messages_t::TypeToEnum<Ty>())
	{
		data_ = line;
	}

	std::size_t length() const
	{
		return header_length + body_length_;
	}

	template<class Ty>
	Ty get()
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
		message_type_ = messages_t::message(std::atoi(header));
		if (message_type_ > messages_t::null_value) {
			return false;
		}
		return true;
	}


	bool decode_body(char* mem)
	{
		const auto dec_fun = [mem](auto Ty) {
			decltype(Ty) buffer;
			std::memcpy(reinterpret_cast<char *>(&buffer), mem, sizeof(buffer));
			return buffer;
		};
		if (message_type_ == messages_t::mes_test) {
			std::array<char, max_body_length> buf;
			std::memcpy(buf.data(), mem, body_length_);
			data_ = buf;
		} else if (message_type_ == messages_t::ShowTurn_e) {
			data_ = dec_fun(ShowTurn{});
        } else if (message_type_ == messages_t::ShowOtherTurn_e) {
            data_ = dec_fun(ShowOtherTurn{});
        } else if (message_type_ == messages_t::Turn_e) {
            data_ = dec_fun(Turn{});
        }

		else log("Зарегистрируйте тип");
		return true;
	}

	void encode_header(char* mem) const
	{
		mem[0] = '\0';
		std::sprintf(mem, "%4u", static_cast<std::uint32_t>(body_length_));
		std::sprintf(mem + 4, "%4u", static_cast<std::uint32_t>(header_type_));
		std::sprintf(mem + 8, "%4u", static_cast<std::uint32_t>(message_type_));
	}

	void encode_body(char* mem) const
	{
		boost::apply_visitor([mem](auto& aut) { encode()(mem, aut); }, data_);
	}

	headers_t::header get_package_type() const
	{
		return header_type_;
	}

private:

	boost::variant<std::array<char, max_body_length>, ShowTurn, ShowOtherTurn, Turn> data_;
	std::size_t body_length_;
	headers_t::header header_type_;
	messages_t::message message_type_;
};


#endif // LABYRINTH_MESSAGE_HPP
