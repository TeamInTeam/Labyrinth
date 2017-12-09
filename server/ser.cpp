/* Copyright (C) 2017 Roman Koshelev */

#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <cstdint>
#include <boost/asio.hpp>
#include "labyrint_message.hpp"
#include <tuple>

#include "../game/game.h"
#include "game_map_scheme.h"

using boost::asio::ip::tcp;

const std::size_t MAX_USERS = 1; // Миш эта константа для тебя

char* getPlayerName(int32_t id)
{
	static char s[64];
	sprintf(s,"player{ %d }", id);
	return s;
}
//----------------------------------------------------------------------

class labyrinth_participant
{
public:
	virtual ~labyrinth_participant() {}
	virtual void deliver(const labyrinth_message& msg) = 0;
	virtual void start_read() = 0;
};

//----------------------------------------------------------------------

class labyrinth_room
{
public:
	void join(std::int32_t id, std::shared_ptr<labyrinth_participant> participant)
	{
		counter_++;
		participants_.emplace(id, participant);
		stat_players_.emplace_back(id, getPlayerName(id));
		if(max_number_users_ == counter_)
		{
			debug_call(game_ , Game(GameMap(Converter(game_map_scheme, 5, 5)), stat_players_));
			stat_players_.clear();
			debug_call(auto vecBuf , game_.StartGame());
			
			assert_debug((vecBuf.first.size() != counter_), "server: Число сообщений не равно числу клиентов\n");
        	deliver2(vecBuf.first);

			get_stroke(vecBuf.second);
		}
	}

	void set_stroke(const Turn & p)  ////////
	{
		debug_call(auto x , game_.TurnHandler(p));
		//printf("%d rr \n", std::get<0>(x).first);
		assert_debug(participants_.find(std::get<0>(x).first) == participants_.end(), "Неверный id клиента\n");
		auto& cl = participants_[std::get<0>(x).first];
		cl->deliver(std::get<0>(x).second);
		//deliver2(std::get<1>(x));
		get_stroke(std::get<2>(x));
	}

	void get_stroke(int32_t id)
	{
		auto& cl = participants_[id];
		cl->start_read();
	}

	void leave(const std::int32_t id)
	{
		counter_--;
		participants_.erase(id);
	}

	void deliver2(const std::vector<std::pair<std::int32_t, ShowTurn>> & vec)
	{
		
		for (const auto& msg : vec) {
			auto& cl = participants_[msg.first];
			cl->deliver(msg.second); //??????????????????
		}
	}

/*	void deliver(const std::vector<std::pair<std::int32_t, labyrinth_message>> & vec)
	{
		for (const auto& msg : vec) {
			auto& cl = participants_[msg.first];
			cl->deliver(msg.second);
		}
	}*/


private:
	std::map<std::int32_t, std::shared_ptr<labyrinth_participant>> participants_;
    std::size_t counter_ = 0;
	Game game_;
	std::vector<ActivePlayer> stat_players_;
	const std::size_t max_number_users_ = MAX_USERS;
};

//----------------------------------------------------------------------

class client
{
public:
	explicit client(tcp::socket socket)
		: client_id_(0), socket_(std::move(socket))
	{
		client_id_ = ((rand() & 0xffff) << 15) | (rand() & 0xffff);
		printf("server: Клиенту присвоен id = %d\n", client_id_);
	}

	tcp::socket& get_socket()
	{
		return socket_;
	}

	std::int32_t get_client_id() const
	{
		return client_id_;
	}
private:
	std::int32_t client_id_;
	tcp::socket socket_;
};

//----------------------------------------------------------------------

class labyrinth_session final
	: public labyrinth_participant,
	public std::enable_shared_from_this<labyrinth_session>
{
public:
	labyrinth_session(tcp::socket socket, labyrinth_room& room)
		: client_(std::move(socket)),
		room_(room)
	{
	}

	void start()
	{
		room_.join(client_.get_client_id(), shared_from_this());
		//do_read_header();
	}

	void start_read() override
	{
		do_read_header();
	}

	void deliver(const labyrinth_message& msg) override
	{
		write_msgs_.push_back(msg);
		do_write();
		//do_read_header();
	}

private:
	void do_read_header()
	{
		const auto self(shared_from_this());
		boost::asio::async_read(client_.get_socket(),
			boost::asio::buffer(clipboard_, labyrinth_message::header_length),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				room_.leave(client_.get_client_id());
				log("server: [0x0002] Клиент отключился");
			} 
			else if(read_msg_.decode_header(clipboard_) && length == labyrinth_message::header_length)
			{
				do_read_body_message();
			} 
			else {
				printf("%zu \n", length );
				log("server: [0x0004] Эмм.... Некорректный пакет. Дебажить клиент!");
				//отправить сокет с плюшками
			}
		});
	}

	void do_read_body_message()
	{
		const auto self(shared_from_this());
		boost::asio::async_read(client_.get_socket(),
			boost::asio::buffer(clipboard_, read_msg_.body_length()),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec && read_msg_.decode_body(clipboard_) && length == read_msg_.body_length())
			{
				fprintf(stderr, "%s%d%s\n", "server: [0x0007] Клиент (id = ", client_.get_client_id(), ") прислал сообщение");
				//room_.deliver({ {client_.get_client_id(), read_msg_  } });
				
				room_.set_stroke(read_msg_.get<Turn>());
			}
			else
			{
				room_.leave(client_.get_client_id());
				log("server: [0x0003] Эмм.... Некорректный пакет. Дебажить клиент!");
			}
		});
	}

	void do_write()
	{
		
		const auto self(shared_from_this());
		write_msgs_.front().encode_header(clipboard_); 
		write_msgs_.front().encode_body(clipboard_ + labyrinth_message::header_length);
		boost::asio::async_write(client_.get_socket(),
			boost::asio::buffer(clipboard_, write_msgs_.front().length()),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			assert(length == write_msgs_.front().length());  // на всякий случай
			//log("server: [0x0007] Клиенту отправлено сообщение");
			fprintf(stderr, "%s%d%s\n", "server: [0x0007] Клиенту (id = ", client_.get_client_id(), ") отправлено сообщение");
			if (!ec)
			{
				write_msgs_.pop_front();
			}
			else
			{
				room_.leave(client_.get_client_id());
				log("server: [0x0002] Клиент отключился");
			}
		});
	}
	
	client client_;
	labyrinth_room& room_;
	labyrinth_message read_msg_;
	std::deque<labyrinth_message> write_msgs_;
	char clipboard_[labyrinth_message::header_length + labyrinth_message::max_body_length];
};

//----------------------------------------------------------------------

class labyrinth_server
{
public:
	labyrinth_server(boost::asio::io_service& io_service,
		const tcp::endpoint& endpoint)
		: acceptor_(io_service, endpoint),
		socket_(io_service)
	{
		do_accept();
	}

private:
	void do_accept()
	{
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			log("server: [0x0001] Клиент подключился");
			if (!ec)
			{
				std::make_shared<labyrinth_session>(std::move(socket_), room_)->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;  //акцептор – объект для приема клиентских подключений
	tcp::socket socket_;
	labyrinth_room room_;
};

//----------------------------------------------------------------------

int main()
{
	setlocale(LC_ALL, "Russian");
	try
	{
		boost::asio::io_service io_service;

		const tcp::endpoint endpoint(tcp::v4(), 2000);
		labyrinth_server server(io_service, endpoint);
		log("server: [0x0005] Старт сервера");
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
