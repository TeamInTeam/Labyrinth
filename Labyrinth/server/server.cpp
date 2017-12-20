/* Copyright (C) 2017 Roman Koshelev */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0501
#endif

#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <cstdint>
#include <tuple>
#include <mutex>

#include <boost/asio.hpp>

#include "./network/labyrint_message.hpp"
#include "./network/logs_and_debug.hpp"


#include "./game-mechanics/game.h"
#include "./DB/game_map_scheme.h"


using boost::asio::ip::tcp;

char* getPlayerName(const int32_t id)
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
	virtual void start_game() = 0;
	virtual void end_game() = 0;
	virtual void deliver(const labyrinth_message& msg) = 0;
	virtual void start_read() = 0;
};

//----------------------------------------------------------------------

class labyrinth_room
	: public std::enable_shared_from_this<labyrinth_room>
{
public:
	labyrinth_room(std::string name, const std::size_t max_users)
		: room_name_(name), max_number_users_(max_users)
	{

	}

	bool join(std::int32_t id, labyrinth_participant* participant)
	{
		counter_++;
		participants_.emplace(id, participant);
		stat_players_.emplace_back(id, getPlayerName(id));
		if (max_number_users_ == counter_)
		{
			start_play();
			return true;
		}
		return false;
	}

	void set_stroke(const labyrinth_message & p)  ////////
	{
		debug_call(auto x, game_.TurnHandler(p.get<Turn>()));

		deliver(std::get<0>(x).first, { std::get<0>(x).second, headers_t::play_mes });
		for (const auto& msg : std::get<1>(x)) {
			deliver(msg.first, { msg.second,headers_t::play_mes });
		}

		if (std::get<2>(x) > 0) {
			get_stroke(std::get<2>(x));
		}
		else {
			end_play();
		}

	}

	void get_stroke(int32_t id)
	{
		assert_debug(participants_.find(id) == participants_.end(), "Неверный id клиента.\n");
		auto& cl = participants_[id];
		cl->start_read();
	}

	void leave(const std::int32_t id)
	{
		counter_--;
		participants_.erase(id);
	}

	void deliver(const std::int32_t id, const labyrinth_message & mes)
	{
		assert_debug(participants_.find(id) == participants_.end(), "Неверный id клиента.\n");
		auto &cl = participants_[id];
		cl->deliver(mes);
	}

	std::string get_name() const
	{
		return room_name_;
	}
private:
	void end_play()
	{
		for (auto i : participants_) {
			i.second->end_game();
		}
	}

	void start_play()
	{
		for (auto i : participants_) {
			i.second->start_game();
		}
		debug_call(game_, Game(GameMap(Converter(arr2, 12, 12)), stat_players_));
		stat_players_.clear();
		std::vector<ActivePlayer>{}.swap(stat_players_);
		debug_call(auto vecBuf, game_.StartGame());

		assert_debug((vecBuf.first.size() != counter_), "server: Число сообщений не равно числу клиентов\n");

		for (const auto& msg : vecBuf.first) {
			deliver(msg.first, { msg.second, headers_t::play_mes });
		}
		get_stroke(vecBuf.second);
	}

	std::string room_name_;
	std::map<std::int32_t, labyrinth_participant*> participants_;
	std::size_t counter_ = 0;
	Game game_;
	std::vector<ActivePlayer> stat_players_;
	const std::size_t max_number_users_/* = MAX_USERS*/;
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

	std::int32_t get_client_id() const
	{
		return client_id_;
	}

	template<class Fun>
	void do_async_read(Fun fun)
	{
		do_read_header(fun);
	}

	template<class Fun>
	void do_async_write(labyrinth_message msg, Fun fun)
	{
		do_write_mes(msg, fun);
	}
private:

	template<class Fun>
	void do_read_header(Fun fun)
	{
		boost::asio::async_read(socket_,
			boost::asio::buffer(clipboard_, labyrinth_message::header_length),
			[this, fun](boost::system::error_code ec, std::size_t length)
		{
			labyrinth_message msg;
			if (ec || length != labyrinth_message::header_length)
			{
				log("server: [0x0002] Клиент отключился or Некорректный пакет.");
			}
			else if (msg.decode_header(clipboard_))
			{
				do_read_body(msg, fun);
			}
		});
	}

	template<class Fun>
	void do_read_body(labyrinth_message msg, Fun fun)
	{
		boost::asio::async_read(socket_,
			boost::asio::buffer(clipboard_, msg.body_length()),
			[this, msg, fun](boost::system::error_code ec, std::size_t length) mutable 
		{
			if (!ec && msg.decode_body(clipboard_) && length == msg.body_length())
			{
				fprintf(stderr, "%s%d%s\n", "server: [0x0007] Клиент (id = ", client_id_, ") прислал сообщение");
				fun(msg, false);
			}
			else
			{
				fun({}, true);
				log("server: [0x0003] Эмм.... Некорректный пакет. Дебажить клиент!");
			}
		});
	}

	template<class Fun>
	void do_write_mes(labyrinth_message msg, Fun fun)
	{
		msg.encode_header(clipboard_);
		msg.encode_body(clipboard_ + labyrinth_message::header_length);
		boost::asio::async_write(socket_,
			boost::asio::buffer(clipboard_, msg.length()),
			[this, msg, fun](boost::system::error_code ec, std::size_t length)
		{
			assert(length == msg.length());  // на всякий случай
			fprintf(stderr, "%s%d%s\n", "server: [0x0007] Клиенту (id = ", client_id_, ") отправлено сообщение");
			if (!ec)
			{
				fun(false);
			}
			else
			{
				fun(true);
				log("server: [0x0002] Клиент отключился");
			}
		});
	}

	std::int32_t client_id_;
	tcp::socket socket_;
	char clipboard_[labyrinth_message::header_length + labyrinth_message::max_body_length];
};

//----------------------------------------------------------------------

class list_room
{
	std::map<std::shared_ptr<labyrinth_room>, std::int32_t> rooms_;

public:

	std::shared_ptr<labyrinth_room> get_room(std::int32_t id)
	{
		for (auto i : rooms_) {
			if (i.second == id) {
				return i.first;
			}
		}
		return nullptr;
	}

	list_room_package get_list_room_package()
	{
		list_room_package l;
		l.vec.reserve(rooms_.size());
		for (auto i : rooms_) {
			l.vec.push_back({ i.second , i.first->get_name() });
		}
		return l;
	}

	void add_room(std::shared_ptr<labyrinth_room> room)
	{
		std::int32_t id= ((rand() & 0xffff)<<15) | (rand() & 0xffff);
		//printf("rrrrrrrrrrrr %u\n", id);
		rooms_[room] = id;
	}

	void del_room(std::shared_ptr<labyrinth_room> room)
	{
		rooms_.erase(room);
	}
};

//----------------------------------------------------------------------

class labyrinth_session final
	: public labyrinth_participant,
	public std::enable_shared_from_this<labyrinth_session>
{
	enum class state_player
	{
		no_authorized,
		authorized,
		is_playing,
	};

public:
	labyrinth_session(tcp::socket socket, list_room& list_r)
		: list_room_(list_r), client_(std::move(socket)), room_(nullptr), state_(state_player::authorized)
	{
	}

	void start_game() override
	{
		state_ = state_player::is_playing;
	}

	void end_game() override
	{
		state_ = state_player::authorized;
		room_ = nullptr;
		start_read();
	}

	void start()
	{
		start_read();
	}

	void start_read() override
	{
		client_.do_async_read([this](labyrinth_message msg, bool error)
		{
			if (!error) 
			{

				if (msg.get_package_type() == headers_t::create_room)
				{
					auto tmp = msg.get<Start_room>();
					room_ = std::make_shared<labyrinth_room>(tmp.room_name_, tmp.count_players_);
					list_room_.add_room(room_);
					bool fl = room_->join(client_.get_client_id(), this);
					if (fl) {
						list_room_.del_room(room_);
					}
				}
				else if (msg.get_package_type() == headers_t::get_list_rooms) 
				{
					labyrinth_message tmp{ list_room_.get_list_room_package(), headers_t::list_rooms };
					deliver(tmp);
					start_read();
					//printf("xx");
					//get_list_room_package
				}
				else if (msg.get_package_type() == headers_t::join_room) {
					auto tmp = msg.get<join_room>();
					room_ = list_room_.get_room(tmp.id_);
					if (room_ != nullptr) {
						bool fl = room_->join(client_.get_client_id(), this);
						if (fl) {
							list_room_.del_room(room_);
						}
					}
					else {
						printf("Вход в несуществующую комнату\n");
					}
				}
				else if (msg.get_package_type() == headers_t::play_mes)
				{
						room_->set_stroke(msg);
						bool fl = room_->join(client_.get_client_id(), this);
						if (fl) {
							list_room_.del_room(room_);
						}
				}
			}
			else
			{
				if (state_ == state_player::is_playing) {
					room_->leave(client_.get_client_id());
				}
			}
		});
	}

	void deliver(const labyrinth_message& msg) override
	{
		do_write(msg);
	}

private:

	void do_write(const labyrinth_message& msg)
	{
		client_.do_async_write(msg, [this](bool error)
		{
			if(!error) {
				//...
			}
			else {
				room_->leave(client_.get_client_id());
			}
		});
	}

	list_room &list_room_;
	client client_;
	std::shared_ptr<labyrinth_room> room_;
	state_player state_;
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
				auto tmp = all_session_.insert(std::make_shared<labyrinth_session>(std::move(socket_), list_room_));
				(*tmp.first)->start();
			}
			do_accept();
		});
	}
	std::set<std::shared_ptr<labyrinth_session>> all_session_;
	tcp::acceptor acceptor_;  //акцептор – объект для приема клиентских подключений
	tcp::socket socket_;
	
	list_room list_room_;
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
