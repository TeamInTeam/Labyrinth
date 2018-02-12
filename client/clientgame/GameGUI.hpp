// Created by Vladislav Melnikov on 03.12.17.

#ifndef LABTECH_GAMEGUIW_HPP
#define LABTECH_GAMEGUIW_HPP

#include <cstring>
#include <string>
#include <cinttypes>
#include <panel.h>
#include <map>
#include "clientmap/ClientMap.hpp"
#include "AsciiInventory/AsciiInv.hpp"

class GameGUI {

private:

    struct Command {
        std::string name;
        char hot_key;
        bool is_directed;
        TurnTypes type;
        int32_t d_m, d_n;
    };


    // Информация, которая известна игроку о карте
    ClientMap game_map_;
    // Координаты поля, в котором находится игрок
    int32_t cur_m_, cur_n_;
    // ХП
    int32_t health_;
    // Если у игрока есть сокровище, то переменная равна 1, иначе - 0
    bool has_treasure_;
    int bombs_, ammo_, concrete_, aids_;
    std::map<int32_t, OtherPlayer> other_players;
    std::vector<Command> str_command_ = {
            { "Go up", 'w', false, TurnTypes::GO, -1, 0 },
            { "Go left", 'a', false, TurnTypes::GO, 0, -1 },
            { "Go down", 's', false, TurnTypes::GO, 1, 0 },
            { "Go right", 'd', false, TurnTypes::GO, 0, 1 },
            { "Watch up", 'i', false, TurnTypes::WATCH, -1, 0 },
            { "Watch lt", 'j', false, TurnTypes::WATCH, 0, -1 },
            { "Watch rt", 'k', false, TurnTypes::WATCH, 1, 0 },
            { "Watch dn", 'l', false, TurnTypes::WATCH, -0, 1 },
    };
    std::vector<Command> str_move_ = {
            { "Up", 'w', true, TurnTypes::GO, -1, 0 },
            { "Left", 'a', true, TurnTypes::GO, 0, -1 },
            { "Down", 's', true, TurnTypes::GO, 1, 0 },
            { "Right", 'd', true, TurnTypes::GO, 0, 1 },
    };


    WINDOW *w_status_bar, *w_map, *w_menu, *w_inventory,
            *w_avatar, *w_debug_bar;
    PANEL *p_status_bar, *p_map, *p_menu, *p_inventory,
            *p_avatar, *p_debug_bar;
    uint32_t cursor_;


    void do_watch_(ShowTurn turn);
    void do_go_(ShowTurn turn);
    void do_knife_(ShowTurn turn);
    void do_shoot_(ShowTurn turn);
    void do_bomb_(ShowTurn turn);
    void do_concrete_(ShowTurn turn);
    void do_aid_(ShowTurn turn);


    void do_watch_(ShowOtherTurn turn);
    void do_go_(ShowOtherTurn turn);
    void do_knife_(ShowOtherTurn turn);
    void do_shoot_(ShowOtherTurn turn);
    void do_bomb_(ShowOtherTurn turn);
    void do_concrete_(ShowOtherTurn turn);
    void do_aid_(ShowOtherTurn turn);


    void print_inventory_();
    void print_avatar_(int32_t new_hp);
    void print_menu_(std::vector<Command> comds);
    Command move_cursor_(std::vector<Command> comds);
    bool end_game_();


    std::string error_str_(ErrorTypes error);
    std::string turn_str_(TurnTypes turn);
    std::string field_str_(FieldTypes field);


//    void debug(std::string str);
//    void debug(Turn turn);
//    void debug(ShowTurn turn);

public:
    GameGUI(ShowTurn new_game);
    virtual ~GameGUI();

    // Считать с потока ввода информацию о ходе игрока
    Turn send_request();
    // Получить информацию с сервера о результате хода игрока
    bool get_response(ShowTurn turn);
    bool get_response(ShowOtherTurn turn);


    void status(std::string str, bool clear = true);
    void status(int32_t num, bool clear = true);
    void status(ErrorTypes err, bool clear = true);
    void status(TurnTypes turn, bool clear = true);
};


#endif //LABTECH_GAMEGUIW_HPP
