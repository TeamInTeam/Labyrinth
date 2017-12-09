// Created by Vladislav Melnikov on 03.12.17.

#ifndef LABTECH_GAMEGUIW_HPP
#define LABTECH_GAMEGUIW_HPP

#include <cstring>
#include <string>
#include <cinttypes>
#include <panel.h>
#include "ClientMap.hpp"

class GameGUIw {

private:
    // Информация, которая известна игроку о карте
    ClientMap game_map_;
    // Координаты поля, в котором находится игрок
    int32_t cur_m_, cur_n_;
    int32_t health_;
    // Если у игрока есть сокровище, то переменная равна 1, иначе - 0
    bool has_treasure_;

    WINDOW *w_status_bar, *w_map, *w_menu, *w_inventory;
    PANEL *p_status_bar, *p_map, *p_menu, *p_inventory;
    uint32_t cursor_;

    struct Command {
        std::string name;
        char hot_key;
    };

    bool is_directed_(TurnTypes &f);
    bool is_moved_(Command command);

    void print_inventory_();
    void print_menu_(std::vector<Command> comds);
    bool move_cursor_(std::vector<Command> comds);
    bool end_game_();
    std::string error_str_(ErrorTypes error);
    std::string turn_str_(TurnTypes turn);

    std::vector<Command> str_command_ = {
            {"Watch", 'r'},
            {"Move", 'm'},
            {"Knife", 'e'},
            {"Shoot", 'f'},
            {"Set bomb", 'g'},
            {"Build", 'b'},
            {"Heal", 'h'},
            {"Go up", 'w'},
            {"Go left", 'a'},
            {"Go down", 's'},
            {"Go right", 'd'},
    };

    std::vector<Command> str_move_ = {
            {"Up", 'w'},
            {"Left", 'a'},
            {"Down", 's'},
            {"Right", 'd'},
    };

public:
    GameGUIw(ShowTurn new_game);
    virtual ~GameGUIw();

    GameGUIw( GameGUIw& other ) = delete;
    GameGUIw( GameGUIw&& other ) = delete;

    GameGUIw& operator=( const GameGUIw& other ) = delete;
    GameGUIw& operator=( GameGUIw&& other ) = delete;

    // Считать с потока ввода информацию о ходе игрока
    Turn send_request();
    // Получить информацию с сервера о результате хода игрока
    bool get_response(ShowTurn turn);
    void status(std::string str, bool clear = true);
    void status(int32_t num, bool clear = true);
    void statuse(ErrorTypes err, bool clear = true);
    void status(TurnTypes turn, bool clear = true);
};


#endif //LABTECH_GAMEGUIW_HPP
