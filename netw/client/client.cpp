/* Copyright (C) 2017 Roman Koshelev */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0501
#endif

#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/variant.hpp>

#include "../server/game-mechanics/proto/proto.h"
#include "../server/network/labyrint_message.hpp"
#include "../server/network/logs_and_debug.hpp"


//-----------------------------------------------------------------------------------------

using boost::asio::ip::tcp;

class labyrinth_client
{
public:
	labyrinth_client(boost::asio::io_service& io_service,
		const tcp::resolver::iterator endpoint_iterator)
		: io_service_(io_service), socket_(io_service) //, timer_(io_service)
	{
		do_connect(endpoint_iterator);
	}

	labyrinth_message do_read()
	{
		labyrinth_message read_msg;
		do_read_header(read_msg);
		return read_msg;
	}

	void do_write(labyrinth_message& msg)
	{
		do_write_mes(msg);
	}

	void client_terminate()
	{
		socket_.close();
		throw std::runtime_error("Сервер упал");
	}

	~labyrinth_client()
	{
		socket_.close();
		log("client: [0x0008] Клиент выключен");
	}

	
private:

	void do_write_mes(labyrinth_message &msg)
	{
		msg.encode_header(clipboard_);
		msg.encode_body(clipboard_ + labyrinth_message::header_length);
		boost::system::error_code ec;
		boost::asio::write(socket_,
			boost::asio::buffer(clipboard_, msg.length()),
			ec);

		if (!ec)
		{
			log("client: [0x0006] Сообщение успешно отправлено");
		}
		else
		{
			log("client: [0x0004] Сервер не отвечает (скорее всего упал)");
			client_terminate();
		}
	}

	void do_connect(const tcp::resolver::iterator endpoint_iterator)
	{
		boost::system::error_code ec;
		boost::asio::connect(socket_, endpoint_iterator, ec);
		if (!ec)
		{
			log("client: [0x0008] Клиент запущен");
		}
		else
		{
			log("client: [0x0001] Сервер не отвечает (включите сервер)");
			client_terminate();
		}
	}

	void do_read_header(labyrinth_message &read_msg)
	{
		boost::system::error_code ec;
		boost::asio::read(socket_,
			boost::asio::buffer(clipboard_, labyrinth_message::header_length),
			boost::asio::transfer_exactly(labyrinth_message::header_length),
			ec);

		if (!ec && read_msg.decode_header(clipboard_))
		{
			do_read_body(read_msg);
		}
		else
		{
			log("client: [0x0002] Сервер не отвечает (скорее всего упал)");
			client_terminate();
		}

	}

	void do_read_body(labyrinth_message &read_msg)
	{
		boost::system::error_code ec;
		boost::asio::read(socket_,
			boost::asio::buffer(clipboard_, read_msg.body_length()),
			boost::asio::transfer_exactly(read_msg.body_length()),
			ec);
		if (!ec && read_msg.decode_body(clipboard_))
		{
			log("client: [0x0007] Сообщение успешно получено");
		}
		else
		{
			log("client: [0x0003] Эмм.... Так не бывает. Дебажить и еще раз дебажить!");
			client_terminate();
		}
	}

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	char clipboard_[labyrinth_message::header_length + labyrinth_message::max_body_length];
};

//-----------------------------------------------------------------------------------------

//----------------------------------------------------------------------

int main() {
	setlocale(LC_ALL, "Russian");

	try {
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		const auto endpoint_iterator = resolver.resolve({ "127.0.0.1", "2000" });
		labyrinth_client c(io_service, endpoint_iterator);
		
		
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

//----------------------------------------------------------------------
