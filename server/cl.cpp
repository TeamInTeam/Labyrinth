/* Copyright (C) 2017 Roman Koshelev */

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "labyrint_message.hpp"
#include "../game/game.h"

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

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

	void do_write(const labyrinth_message& msg)
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

	void do_write_mes(const labyrinth_message& msg)
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
//	boost::asio::deadline_timer timer_;
};

//----------------------------------------------------------------------

int main()
{
	setlocale(LC_ALL, "Russian");
	try
	{
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		const auto endpoint_iterator = resolver.resolve({ "127.0.0.1", "2000" });

		labyrinth_client c(io_service, endpoint_iterator);

		char line[labyrinth_message::max_body_length + 1];
		
		//unsigned int start_time = clock(); // начальное время

        /*
		while (std::cin.getline(line, labyrinth_message::max_body_length + 1))
		{
			labyrinth_message msg(line, headers_t::test_mes, messages_t::mes_test);
			//c.do_write(msg);
			msg = c.do_read();
			msg.write();
		}
        */

		/*unsigned int end_time = clock(); // конечное время
		unsigned int search_time = end_time - start_time;
		std::cout << '\n' << "runtime = " << search_time / 1000. << std::endl;*/
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	
	return 0;
}

//----------------------------------------------------------------------
