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

struct ShowActivePlayer {
    ShowActivePlayer();
    
    int32_t player_id;
    std::string player_name;

    bool is_my_turn;

    int32_t health;

    bool has_previous_treasure;
    bool has_treasure;
    
    int32_t my_bombs;
    int32_t my_ammo;
    int32_t my_concrete;
    int32_t my_aids;

    int32_t my_m, my_n;
    FieldTypes type_of_field;
    
    std::vector<std::vector<std::string>> graphics;
};

ShowActivePlayer::ShowActivePlayer() :
    player_id(0),
    player_name(""),
    is_my_turn(false),
    health(0),
    has_previous_treasure(false),
    has_treasure(false),
    my_bombs(0),
    my_ammo(0),
    my_concrete(0),
    my_aids(0),
    my_m(-1),
    my_n(-1),
    type_of_field(FieldTypes::ROAD) {}

void Graphics(ShowActivePlayer show_active_player);

Turn Input(ShowActivePlayer &show_active_player);

void ForShowTurn(ShowActivePlayer &show_active_player, ShowTurn show_turn);

void ShowTurnWatch(ShowActivePlayer &show_active_player, ShowTurn show_turn);

void ShowTurnGo(ShowActivePlayer &show_active_player, ShowTurn show_turn);

void ShowTurnKnife(ShowActivePlayer &show_active_player, ShowTurn show_turn);

void ShowTurnBomb(ShowActivePlayer &show_active_player, ShowTurn show_turn);

void ShowTurnConcrete(ShowActivePlayer &show_active_player, ShowTurn show_turn);

void ShowTurnAid(ShowActivePlayer &show_active_player, ShowTurn show_turn);

void ForShowOtherTurn(ShowActivePlayer &show_active_player, ShowOtherTurn show_turn);

void ShowOtherTurnGo(ShowActivePlayer &show_active_player, ShowOtherTurn show_turn);

//----------------------------------------------------------------------
//////////////  Консольная тесстовая графика
int main() {
	setlocale(LC_ALL, "Russian");
	std::string tmp_str2 = "127.0.0.1";
	/*std::cout << "Введите ip: \n";
	std::cin >> tmp_str2;*/
	try {
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		const auto endpoint_iterator = resolver.resolve({ tmp_str2.c_str(), "2000" });
		labyrinth_client c(io_service, endpoint_iterator);
		
		std::cout << "Введите команду: \n" << "1 - создать комнату\n"<< "2 - получить список комнат\n";
		int com = 0;
		std::cin >> com;
		if (com == 1) {
			std::string tmp_str;
			std::cout << "Введите название комнаты: \n";
			std::cin >> tmp_str;
			std::uint32_t com2 = 0;
			std::cout << "Введите количество игроков: \n";
			std::cin >> com2;
			auto mes2 = labyrinth_message{ Start_room{ com2, tmp_str.c_str() } , headers_t::create_room };//
			c.do_write(mes2);
		} 
		else if(com == 2) {
			auto mes2 = labyrinth_message{ get_list_room{} , headers_t::get_list_rooms };
			c.do_write(mes2);
			mes2 = c.do_read();
			auto x = mes2.get<list_room_package>();
			for (auto i = x.vec.begin(); i != x.vec.end(); ++i)
			{
				printf("%lld %s\n", i - x.vec.begin(), i->second.data());
			}
			std::uint32_t com2 = 0;
			std::cout << "Введите номер комнаты: \n";
			std::cin >> com2;

			com2 = (x.vec.begin() + com2)->first;
			mes2 = labyrinth_message{ join_room{ com2 } , headers_t::join_room };
			c.do_write(mes2);
		} 
		else {
			return 0;
		}
		
		auto mes = c.do_read();
		ShowTurn start = mes.get<ShowTurn>();
        
        ShowActivePlayer show_active_player;
        show_active_player.graphics.resize(start.size_m);
        for (size_t j = 0; j < show_active_player.graphics.size(); j++) {
            show_active_player.graphics[j].resize(start.size_n);
            for (size_t k = 0; k < show_active_player.graphics[j].size(); k++)
                show_active_player.graphics[j][k] = "*";
        }
        
        show_active_player.is_my_turn = start.my_turn;
        show_active_player.my_m = start.m;
        show_active_player.my_n = start.n;
        show_active_player.graphics[start.m][start.n] = "@";
        show_active_player.player_name = "Asuka Langley";
        show_active_player.health = start.health;

        Graphics(show_active_player);

      //  std::system("mpg123 -q ~/rock.mp3 &");

		while (true) {
            if (show_active_player.is_my_turn) {
                auto turn = Input(show_active_player);
                auto mes2 = labyrinth_message{ turn,headers_t::play_mes };
                c.do_write(mes2);
                mes2 = c.do_read();
                auto buf = mes2.get<ShowTurn>();
                if (buf.game_end) {
                    std::cout << "____________" << std::endl;
                    std::cout << "|You win!!!|" << std::endl;
                    std::cout << "____________" << std::endl;
                    std::cout << std::endl << std::endl;
                    exit(0);
                }
                ForShowTurn(show_active_player, buf);
                show_active_player.is_my_turn = buf.my_turn;
            } else {
                auto mes2 = c.do_read();
                auto buf = mes2.get<ShowOtherTurn>();
                if (buf.game_end) {
                    std::cout << "_____________" << std::endl;
                    std::cout << "|You lose!!!|" << std::endl;
                    std::cout << "_____________" << std::endl;
                    std::cout << std::endl << std::endl;
                    exit(0);
                }
                ForShowOtherTurn(show_active_player, buf);
                show_active_player.is_my_turn = buf.my_turn;
            }
        }
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

//----------------------------------------------------------------------

void Graphics(ShowActivePlayer show_active_player) {
    auto graphics = show_active_player.graphics;
#ifdef _MSC_VER
	system("cls");
#else
	std::cout << "\033[2J\033[1;1H";
#endif
	for (size_t i = 0; i < graphics[0].size() + 1; i++)
		std::cout << "__";
	std::cout << std::endl;
	for (size_t i = 0; i < graphics.size(); i++) {
		std::cout << "|";
		for (size_t j = 0; j < graphics[i].size(); j++)
			std::cout << graphics[i][j] << " ";
		std::cout << "|" << std::endl;
	}
	for (size_t i = 0; i < graphics[0].size() + 1; i++)
		std::cout << "__";
	std::cout << std::endl;
    if (show_active_player.has_treasure)
		std::cout << "I have treasure!" << std::endl;
    std::cout << "I have ammo: " << show_active_player.my_ammo << std::endl;
    std::cout << "I have bombs: " << show_active_player.my_bombs << std::endl;
    std::cout << "I have concrete: " << show_active_player.my_concrete << std::endl;
    std::cout << "I have aids:" << show_active_player.my_aids << std::endl;
}

Turn Input(ShowActivePlayer &show_active_player) {
    Turn turn;
    while (true) {
        std::cout << "command: ";
        std::string command;
        std::cin >> command;
        if (command == "watch") {
            turn.turn = TurnTypes::WATCH;
        }
        if (command == "go") {
            turn.turn = TurnTypes::GO;
        }
        if (command == "knife") {
            turn.turn = TurnTypes::KNIFE;
        }
        if (command == "bomb") {
            turn.turn = TurnTypes::BOMB;
        }
        if (command == "conc") {
            turn.turn = TurnTypes::CONCRETE;
        }
        if (command == "aid") {
            turn.turn = TurnTypes::AID;
        }
        std::string direction;
        if (command != "aid") {
            std::cout << "direction: ";
            std::cin >> direction;
            if (direction == "u") {
                turn.m = show_active_player.my_m - 1;
                turn.n = show_active_player.my_n;
            }
            if (direction == "d") {
                turn.m = show_active_player.my_m + 1;
                turn.n = show_active_player.my_n;
            }
            if (direction == "l") {
                turn.m = show_active_player.my_m;
                turn.n = show_active_player.my_n - 1;
            }
            if (direction == "r") {
                turn.m = show_active_player.my_m;
                turn.n = show_active_player.my_n + 1;
            }
        } else {
            turn.m = show_active_player.my_m;
            turn.n = show_active_player.my_n;
        }
        if ((((command == "watch") || (command == "go") || (command == "knife") || (command == "bomb") || (command == "conc")) &&
             ((direction == "u") || (direction == "d") || (direction == "l") || (direction == "r"))) ||
            (command == "aid")) {
            break;
        }
    }
    return turn;
}

void ForShowTurn(ShowActivePlayer &show_active_player, ShowTurn show_turn) {
	if (show_turn.error != ErrorTypes::NO_ERR_IN_MSG) {
        switch (show_turn.error) {
        case ErrorTypes::NOT_MY_TURN: {
            std::cout << "Error: not my turn!!!" << std::endl;
            break;
        }
        case ErrorTypes::UNKNOWN_COMMAND: {
            std::cout << "Error: unknown command!!!" << std::endl;
            break;
        }
        case ErrorTypes::INVALID_DISTANCE: {
            std::cout << "Error: invalid distance!!!" << std::endl;
            break;
        }
        case ErrorTypes::INVALID_DIRECTION: {
            std::cout << "Error: invalid direction!!!" << std::endl;
            break;
        }
        case ErrorTypes::NO_AMMO: {
            std::cout << "Error: no ammo!!!" << std::endl;
            break;
        }
        case ErrorTypes::NO_BOMB: {
            std::cout << "Error: no bombs!!!" << std::endl;
            break;
        }
        case ErrorTypes::NO_CONCRETE: {
            std::cout << "Error: no concrete!!!" << std::endl;
            break;
        }
        case ErrorTypes::NO_AID: {
            std::cout << "Error: !!!" << std::endl;
            break;
        }
        case ErrorTypes::I_AM_DEAD: {
            std::cout << "Error: I am dead!!!" << std::endl;
            break;
        }
        default: {
            std::cout << "No error!!!" << std::endl;
            break;
        }
        }
	}
	switch (show_turn.turn) {
    case TurnTypes::WATCH: {
        ShowTurnWatch(show_active_player, show_turn);
        break;
    }
	case TurnTypes::GO: {
		ShowTurnGo(show_active_player, show_turn);
		break;
	}
        /*
	case TurnTypes::KNIFE: {
		ShowTurnKnife(show_active_player, show_turn);
		break;
	}
	case TurnTypes::BOMB: {
		ShowTurnBomb(show_active_player, show_turn);
		break;
	}
	case TurnTypes::CONCRETE: {
		ShowTurnConcrete(show_active_player, show_turn);
		break;
	}
	case TurnTypes::AID: {
		ShowTurnAid(show_active_player, show_turn);
		break;
	}
        */
	default: {
		std::cout << "ERROR, UNKNOWN COMMAND!!!" << std::endl;
		exit(1);
	}
	}
}

void ShowTurnWatch(ShowActivePlayer &show_active_player, ShowTurn show_turn) {
    switch (show_turn.field) {
	case FieldTypes::ROAD: {
        show_active_player.graphics[show_turn.m][show_turn.n] = " ";
		break;
	}
	case FieldTypes::RIVER: {
        show_active_player.graphics[show_turn.m][show_turn.n] = "~";
		break;
	}
	case FieldTypes::METRO: {
        show_active_player.graphics[show_turn.m][show_turn.n] = "M";
		break;
	}
    case FieldTypes::WALL: {
        show_active_player.graphics[show_turn.m][show_turn.n] = "|";
		break;
	}
    case FieldTypes::CONCRETE: {
        show_active_player.graphics[show_turn.m][show_turn.n] = "&";
		break;
	}
	case FieldTypes::ARMOURY: {
        show_active_player.graphics[show_turn.m][show_turn.n] = "A";
		break;
	}
	case FieldTypes::HOSPITAL: {
        show_active_player.graphics[show_turn.m][show_turn.n] = "H";
		break;
	}
	case FieldTypes::BOMB: {
        show_active_player.graphics[show_turn.m][show_turn.n] = "0";
		break;
	}
	}
    if (show_turn.treasure)
        show_active_player.graphics[show_turn.m][show_turn.n] = "T";
    if (show_turn.current_player != PlayerTypes::NO_PLAYER) {
        if (show_turn.current_player == PlayerTypes::DEAD_PLAYER)
            show_active_player.graphics[show_turn.m][show_turn.n] = "@";
        else
            show_active_player.graphics[show_turn.m][show_turn.n] = "#";
    }
    Graphics(show_active_player);
}

void ShowTurnGo(ShowActivePlayer &show_active_player, ShowTurn show_turn) {
    if (((show_turn.m == 0) ||
         (show_turn.n == 0) ||
         (show_turn.m == show_active_player.graphics.size() - 1) ||
         (show_turn.n == show_active_player.graphics[0].size() - 1)) &&
        (!show_active_player.has_treasure) &&
        (show_turn.field == FieldTypes::ROAD)) {
        show_active_player.graphics[show_turn.m][show_turn.n] = " ";
        Graphics(show_active_player);
        return;
    }
	switch (show_active_player.type_of_field) {
	case FieldTypes::ROAD: {
		if (show_active_player.has_previous_treasure) {
			show_active_player.has_previous_treasure = false;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "T";
		} else {
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = " ";
		}
		break;
	}
	case FieldTypes::RIVER: {
        if (show_active_player.has_previous_treasure) {
			show_active_player.has_previous_treasure = false;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "T";
		} else {
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "~";
		}
		break;
	}
	case FieldTypes::METRO: {
        if (show_active_player.has_previous_treasure) {
			show_active_player.has_previous_treasure = false;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "T";
		} else {
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "M";
		}
		break;
	}
	case FieldTypes::ARMOURY: {
        if (show_active_player.has_previous_treasure) {
			show_active_player.has_previous_treasure = false;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "T";
		}
		else {
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "A";
		}
		break;
	}
	case FieldTypes::HOSPITAL: {
        if (show_active_player.has_previous_treasure) {
			show_active_player.has_previous_treasure = false;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "T";
		} else {
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "H";
		}
		break;
	}
	case FieldTypes::BOMB: {
        if (show_active_player.has_previous_treasure) {
			show_active_player.has_previous_treasure = false;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "T";
		} else {
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "0";
		}
		break;
	}
	}
    if (show_turn.previous_player != PlayerTypes::NO_PLAYER) {
        if (show_turn.previous_player == PlayerTypes::DEAD_PLAYER)
            show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "#";
        else
            show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "@";
    }
	switch (show_turn.field) {
	case FieldTypes::ROAD: {
		show_active_player.my_m = show_turn.m;
		show_active_player.my_n = show_turn.n;
		show_active_player.type_of_field = show_turn.field;
		show_active_player.health = show_turn.health;
        show_active_player.has_treasure = show_active_player.has_treasure || (show_turn.treasure && show_turn.health > 0);
		show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
		break;
	}
	case FieldTypes::WALL: {
		show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
		show_active_player.graphics[show_turn.m][show_turn.n] = "|";
		break;
	}
	case FieldTypes::RIVER: {
		for (size_t i = 0; i < show_turn.river_coordinates.size(); i++) {
			show_active_player.my_m = show_turn.river_coordinates[i].first;
			show_active_player.my_n = show_turn.river_coordinates[i].second;
			show_active_player.type_of_field = show_turn.field;
			show_active_player.health = show_turn.health;
            show_active_player.has_treasure = show_active_player.has_treasure || (show_turn.treasure && show_turn.health > 0);
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
			Graphics(show_active_player);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "~";
		}
        show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
		break;
	}
	case FieldTypes::METRO: {
		show_active_player.graphics[show_turn.m][show_turn.n] = "M";
		show_active_player.my_m = show_turn.metro_m;
		show_active_player.my_n = show_turn.metro_n;
		show_active_player.type_of_field = show_turn.field;
		show_active_player.health = show_turn.health;
        show_active_player.has_treasure = show_active_player.has_treasure || (show_turn.treasure && show_turn.health > 0);
		show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
		break;
	}
	case FieldTypes::HOSPITAL: {
		show_active_player.my_m = show_turn.m;
		show_active_player.my_n = show_turn.n;
		show_active_player.type_of_field = show_turn.field;
		show_active_player.health = show_turn.health;
        show_active_player.my_aids = show_turn.aids;
        show_active_player.has_treasure = show_active_player.has_treasure || (show_turn.treasure && show_turn.health > 0);
		show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
		break;
	}
	case FieldTypes::ARMOURY: {
		show_active_player.my_m = show_turn.m;
		show_active_player.my_n = show_turn.n;
		show_active_player.type_of_field = show_turn.field;
		show_active_player.health = show_turn.health;
        show_active_player.my_bombs = show_turn.bombs;
        show_active_player.my_ammo = show_turn.ammo;
        show_active_player.my_concrete = show_turn.concrete;
        show_active_player.my_aids = show_turn.aids;
        show_active_player.has_treasure = show_active_player.has_treasure || (show_turn.treasure && show_turn.health > 0);
		show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
		break;
	}
	case FieldTypes::BOMB: {
		show_active_player.my_m = show_turn.m;
		show_active_player.my_n = show_turn.n;
		if (show_active_player.health > 0) {
			show_active_player.health = 0;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "0";
			Graphics(show_active_player);
			show_active_player.type_of_field = FieldTypes::ROAD;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "#";
			show_active_player.my_bombs = show_active_player.my_ammo = show_active_player.my_concrete = show_active_player.my_aids = 0;
			if (show_active_player.has_treasure)
				show_active_player.has_previous_treasure = true;
			show_active_player.has_treasure = false;
			Graphics(show_active_player);
		} else {
			show_active_player.type_of_field = FieldTypes::BOMB;
			show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = "#";
		}
		break;
	}
	case FieldTypes::CONCRETE: {
		show_active_player.graphics[show_active_player.my_m][show_active_player.my_n] = show_active_player.health > 0 ? "@" : "#";
		show_active_player.graphics[show_turn.m][show_turn.n] = "&";
		break;
	}
	}
	Graphics(show_active_player);
}

/*
void ShowTurnKnife(ShowActivePlayer &show_active_player, ShowTurn show_turn) {
	Graphics(show_active_player);

	if (show_turn.player) {
		std::cout << "You hurt player!" << std::endl;
	}
	else {
		std::cout << "You didn't hurt player!" << std::endl;
	}
	std::cout << std::endl;
}

void ShowTurnBomb(ShowActivePlayer &show_active_player, ShowTurn show_turn) {
	switch (show_turn.field) {
	case FieldTypes::ROAD: {
		if (!show_turn.player) {
			Graphics(show_active_player);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			show_active_player.graphics[show_turn.m][show_turn.n] = "Θ";
			ShowGraphics(show_active_player);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			show_active_player.graphics[show_turn.m][show_turn.n] = " ";
			ShowGraphics(show_active_player);
		}
		else {
			show_active_player.graphics[show_turn.m][show_turn.n] = "@";
		}
		break;
	}
	case FieldTypes::BOMB: {
        if (show_active_player.my_bombs == show_turn.bombs + 1) {
            ShowGraphics(show_active_player);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            show_active_player.graphics[show_turn.m][show_turn.n] = "Θ";
            ShowGraphics(show_active_player);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            show_active_player.my_bombs--;
        } else {
            ShowGraphics(show_active_player);
            std::cout << "Unable to place bomb!!!" << std::endl;
        }
		break;
	}
	default: {
		ShowGraphics(show_active_player);
		std::cout << "Unable to place bomb!!!" << std::endl;
		break;
	}
	}

    if (show_turn.treasure && show_active_player.health > 0) {
		show_active_player.has_treasure = true;
	}
	if (show_active_player.has_treasure)
		std::cout << "I have treasure!" << std::endl;
    show_active_player.my_ammo = show_turn.ammo;
	std::cout << "I have ammo: " << show_active_player.my_ammo << std::endl;
    show_active_player.my_bombs = show_turn.bombs;
	std::cout << "I have bombs: " << show_active_player.my_bombs << std::endl;
    show_active_player.my_concrete = show_turn.concrete;
	std::cout << "I have concrete: " << show_active_player.my_concrete << std::endl;
    show_active_player.my_aids = show_turn.aids;
	std::cout << "I have aids:" << show_active_player.my_aids << std::endl;
}

void ShowTurnConcrete(ShowActivePlayer &show_active_player, ShowTurn show_turn) {
	if (show_turn.field == FieldTypes::CONCRETE) {
		Graphics(show_active_player);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		show_active_player.graphics[show_turn.m][show_turn.n] = "¥";
		Graphics(show_active_player);
	}
	else {
		Graphics(show_active_player);
		std::cout << "Unable to place concrete!!!" << std::endl;
	}

    if (show_turn.treasure && show_active_player.health > 0) {
		show_active_player.has_treasure = true;
	}
	if (show_active_player.has_treasure)
		std::cout << "I have treasure!" << std::endl;
    show_active_player.my_ammo = show_turn.ammo;
	std::cout << "I have ammo: " << show_active_player.my_ammo << std::endl;
    show_active_player.my_bombs = show_turn.bombs;
	std::cout << "I have bombs: " << show_active_player.my_bombs << std::endl;
    show_active_player.my_concrete = show_turn.concrete;
	std::cout << "I have concrete: " << show_active_player.my_concrete << std::endl;
    show_active_player.my_aids = show_turn.aids;
	std::cout << "I have aids:" << show_active_player.my_aids << std::endl;
}

void ShowTurnAid(ShowActivePlayer &show_active_player, ShowTurn show_turn) {
	if (show_active_player.health != show_turn.health) {
		show_active_player.health = show_turn.health;
		show_active_player.my_aids--;
	}
	Graphics(show_active_player);
    if (show_turn.treasure && show_active_player.health > 0) {
		show_active_player.has_treasure = true;
	}
	if (show_active_player.has_treasure)
		std::cout << "I have treasure!" << std::endl;
    show_active_player.my_ammo = show_turn.ammo;
	std::cout << "I have ammo: " << show_active_player.my_ammo << std::endl;
    show_active_player.my_bombs = show_turn.bombs;
	std::cout << "I have bombs: " << show_active_player.my_bombs << std::endl;
    show_active_player.my_concrete = show_turn.concrete;
	std::cout << "I have concrete: " << show_active_player.my_concrete << std::endl;
    show_active_player.my_aids = show_turn.aids;
	std::cout << "I have aids:" << show_active_player.my_aids << std::endl;
}
*/

void ForShowOtherTurn(ShowActivePlayer &show_active_player, ShowOtherTurn show_turn) {
    switch (show_turn.turn) {
    case TurnTypes::WATCH: {
		break;
	}
	case TurnTypes::GO: {
		ShowOtherTurnGo(show_active_player, show_turn);
		break;
	}
	default: {
		std::cout << "ERROR, UNKNOWN COMMAND!!!" << std::endl;
		exit(1);
	}
	}
}

void ShowOtherTurnGo(ShowActivePlayer &show_active_player, ShowOtherTurn show_turn) {
    if (show_turn.visible_from) {
        switch (show_turn.previous_field) {
        case FieldTypes::ROAD: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = " ";
            break;
        }
        case FieldTypes::WALL: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "|";
            break;
        }
        case FieldTypes::CONCRETE: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "&";
            break;
        }
        case FieldTypes::RIVER: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "~";
            break;
        }
        case FieldTypes::METRO: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "M";
            break;
        }
        case FieldTypes::ARMOURY: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "A";
            break;
        }
        case FieldTypes::HOSPITAL: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "H";
            break;
        }
        case FieldTypes::BOMB: {
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "0";
            break;
        }
        }
        if (show_turn.previous_treasure)
            show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "T";
        if (show_turn.previous_player != PlayerTypes::NO_PLAYER) {
            if (show_turn.previous_player == PlayerTypes::DEAD_PLAYER)
                show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "#";
            else
                show_active_player.graphics[show_turn.previous_m][show_turn.previous_n] = "@";
        }
    }
    if (show_turn.visible_to) {
        switch (show_turn.current_field) {
        case FieldTypes::ROAD: {
            show_active_player.graphics[show_turn.current_m][show_turn.current_n] = " ";
            break;
        }
        case FieldTypes::WALL: {
            show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "|";
            break;
        }
        case FieldTypes::CONCRETE: {
            show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "&";
            break;
        }
        case FieldTypes::RIVER: {
            for (size_t i = 0; i < show_turn.river_coordinates.size(); i++) {
                if (show_turn.current_player == PlayerTypes::DEAD_PLAYER)
                    show_active_player.graphics[show_turn.river_coordinates[i].first][show_turn.river_coordinates[i].second] = "#";
                else
                    show_active_player.graphics[show_turn.river_coordinates[i].first][show_turn.river_coordinates[i].second] = "@";
                Graphics(show_active_player);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                show_active_player.graphics[show_turn.river_coordinates[i].first][show_turn.river_coordinates[i].second] = "~";
                Graphics(show_active_player);
            }
            if (show_turn.current_player != PlayerTypes::NO_PLAYER) {
                if (show_turn.current_player == PlayerTypes::DEAD_PLAYER)
                    show_active_player.graphics[show_turn.river_coordinates[show_turn.river_coordinates.size() - 1].first][show_turn.river_coordinates[show_turn.river_coordinates.size() - 1].second] = "#";
                else
                    show_active_player.graphics[show_turn.river_coordinates[show_turn.river_coordinates.size() - 1].first][show_turn.river_coordinates[show_turn.river_coordinates.size() - 1].second] = "@";
                break;
            }
        }
        case FieldTypes::METRO: {
            show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "M";
            if (show_turn.metro_m != -1) {
                if (show_turn.current_player == PlayerTypes::DEAD_PLAYER)
                    show_active_player.graphics[show_turn.metro_m][show_turn.metro_n] = "#";
                else
                    show_active_player.graphics[show_turn.metro_m][show_turn.metro_n] = "@";
            }
            break;
        }
        case FieldTypes::ARMOURY: {
            show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "A";
            break;
        }
        case FieldTypes::HOSPITAL: {
            show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "H";
            break;
        }
        case FieldTypes::BOMB: {
            if (show_turn.current_player == PlayerTypes::DEAD_PLAYER) {
                show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "#";
            } else {
                show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "@";
                Graphics(show_active_player);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "0";
                Graphics(show_active_player);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "#";
                Graphics(show_active_player);
            }
            break;
        }
        }
        if (show_turn.current_treasure) {
            show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "T";
        }
        if (show_turn.current_field != FieldTypes::RIVER && show_turn.current_field != FieldTypes::METRO && show_turn.current_field != FieldTypes::BOMB) {
            if (show_turn.current_player != PlayerTypes::NO_PLAYER) {
                if (show_turn.current_player == PlayerTypes::DEAD_PLAYER)
                    show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "#";
                else
                    show_active_player.graphics[show_turn.current_m][show_turn.current_n] = "@";
            }
        }
    }
    Graphics(show_active_player);
}
