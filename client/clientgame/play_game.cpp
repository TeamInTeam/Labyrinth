// Created by Vladislav Melnikov on 19.12.17.

#include "play_game.hpp"

/* Copyright (C) 2017 Roman Koshelev */

#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "../network/labyrint_message.hpp"
#include "../network/logs_and_debug.hpp"
#include "boost/variant.hpp"

//-----------------------------------------------------------------------------------------

using boost::asio::ip::tcp;

class labyrinth_client {
public:
    labyrinth_client(boost::asio::io_service& io_service,
                     const tcp::resolver::iterator endpoint_iterator)
            : io_service_(io_service), socket_(io_service) {
        do_connect(endpoint_iterator);
    }

    labyrinth_message do_read() {
        labyrinth_message read_msg;
        do_read_header(read_msg);
        return read_msg;
    }

    void do_write(labyrinth_message& msg) {
        do_write_mes(msg);
    }

    void client_terminate() {
        socket_.close();
        throw std::runtime_error("Сервер упал");
    }

    ~labyrinth_client() {
        socket_.close();
        log("client: [0x0008] Клиент выключен");
    }

private:

    void do_write_mes(labyrinth_message &msg) {
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

    void do_connect(const tcp::resolver::iterator endpoint_iterator) {
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

    void do_read_header(labyrinth_message &read_msg) {
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

    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    char clipboard_[labyrinth_message::header_length + labyrinth_message::max_body_length];
};


// MEGAGOVNOKOD
void start_game(Info info) {



    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        const auto endpoint_iterator = resolver.resolve({ "127.0.0.1", "2000" });
        labyrinth_client c(io_service, endpoint_iterator);

        int maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);
        int zerox = maxx < 56 ? 21 : (maxx - 13) / 2;
        int zeroy = maxy < 27 ? 8 : (maxy - 12) / 2;
        WINDOW* w = newwin(7, 18, zeroy, zerox);
        wbkgdset(w, PAIR_INV);
        PANEL* p = new_panel(w);
        top_panel(p);
        update_panels();
        doupdate();
        wclear(w);

        if (info.flag) {
            auto message = labyrinth_message{Start_room{info.count, info.name.c_str()}, headers_t::create_room};
            c.do_write(message);
            box(w, ACS_VLINE, ACS_HLINE);
            mvwprintw(w, 3, 4, "!!!WAIT!!!");
            wrefresh(w);
        }
        else {
            auto message = labyrinth_message{get_list_room{}, headers_t::get_list_rooms};
            c.do_write(message);
            message = c.do_read();
            auto list_room = message.get<list_room_package>();
            box(w, ACS_VLINE, ACS_HLINE);
            mvwprintw(w, 0, 1, "List rooms:");
            for (auto i = list_room.vec.begin(); i != list_room.vec.end(); ++i) {
                mvwprintw(w, i - list_room.vec.begin() + 1, 1, "%d. %s",
                          i - list_room.vec.begin(), i->second.c_str());
            }
            wrefresh(w);
            int32_t ch = (list_room.vec.begin() + (getch() - '0'))->first;
            message = labyrinth_message{join_room{ch}, headers_t::join_room};
            c.do_write(message);
        }
        auto mes = c.do_read();
        ShowTurn start_game = mes.get<ShowTurn>();
        bool is_my_turn = start_game.my_turn;
        labyrinth_message client;


        del_panel(p);
        delwin(w);
        update_panels();
        doupdate();

        GameGUI game_gui(start_game);
        bool win = false;

        while (true) {
            if (is_my_turn) {
                auto turn = game_gui.send_request();
                client = labyrinth_message{turn, headers_t::play_mes};
                c.do_write(client);
            }
            client = c.do_read();
            if (is_my_turn) {
                auto buf = client.get<ShowTurn>();
                if (buf.game_end) {
                    win = true;
                    break;
                }
                game_gui.get_response(buf);
                is_my_turn = buf.my_turn;
            }
            else {
                auto buf = client.get<ShowOtherTurn>();
                if (buf.game_end)
                    win = true;
                game_gui.get_response(buf);
                is_my_turn = buf.my_turn;
            }
        }

        maxx = getmaxx(stdscr);
        maxy = getmaxy(stdscr);
        zerox = maxx < 56 ? 21 : (maxx - 13) / 2;
        zeroy = maxy < 27 ? 3 : (maxy - 12) / 2;
        w = newwin(4, 18, zeroy, zerox);
        wbkgdset(w, PAIR_INV);
        p = new_panel(w);
        wclear(w);
        top_panel(p);
        update_panels();
        doupdate();
        mvwprintw(w, 1, 1, win ? "You Win" : "FUUUUU");
        mvwprintw(w, 2, 1, "Press any key...");
        box(w, ACS_VLINE, ACS_HLINE);
        wrefresh(w);
        getch();
        del_panel(p);
        delwin(w);
        update_panels();
        doupdate();
    }
    catch (std::exception& e) {
        int maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);
        int zerox = maxx < 56 ? 21 : (maxx - 13) / 2;
        int zeroy = maxy < 27 ? 8 : (maxy - 12) / 2;
        WINDOW* w = newwin(6, 13, zeroy, zerox);
        wbkgdset(w, PAIR_INV);
        PANEL* p = new_panel(w);
        top_panel(p);
        update_panels();
        doupdate();
        mvwprintw(w, 1, 3, "Server");
        mvwprintw(w, 2, 3, "is not");
        mvwprintw(w, 3, 2, "available");
        wmove(w, 4, 5);
        wattron(w, A_REVERSE);
        wprintw(w, "OK");
        wattroff(w, A_REVERSE);
        box(w, ACS_VLINE, ACS_HLINE);
        wrefresh(w);
        getch();
        del_panel(p);
        delwin(w);
        update_panels();
        doupdate();
        std::cerr << "Exception: " << e.what() << "\n";
    }

}
