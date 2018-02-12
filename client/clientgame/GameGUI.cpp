// Created by Vladislav Melnikov on 03.12.17.

#include "GameGUI.hpp"
#include <chrono>
#include <thread>

template <class E>
constexpr auto to_type(E enumrator) noexcept {
    return static_cast<std::underlying_type_t<E>>(enumrator);
}


template <class E>
constexpr auto from_type(std::underlying_type_t<E> arg) {
    return E(arg);
}


WINDOW* init_w(int32_t size_m, int32_t size_n, int32_t beg_m, int32_t beg_n, int32_t color = 1) {
    WINDOW* w = newwin(size_m, size_n, beg_m, beg_n);
    wbkgdset(w, COLOR_PAIR(color));
    return w;
}


GameGUI::GameGUI(ShowTurn new_game)
        : game_map_(new_game.size_m) {
    cur_m_ = new_game.m;
    cur_n_ = new_game.n;
    health_ = new_game.health;
    has_treasure_ = false;
    bombs_ = aids_ = ammo_ = concrete_ = 0;

    for (auto pl : new_game.other_players)
        if (pl.first != new_game.my_id)
            other_players[pl.first] = {-1, -1, false};

    game_map_[cur_m_][cur_n_].first
            = new_game.field;
    cursor_ = 0;
    int maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);
    int zerox = maxx < 56 ? 2 : (maxx - 54) / 2;
    int zeroy = maxy < 27 ? 1 : (maxy - 26) / 2;
    w_status_bar = init_w(3, 54, zeroy, zerox);
    w_avatar = init_w(7, 9, 4 + zeroy, zerox);
    w_map = init_w(14, 25, 4 + zeroy, 12 + zerox);
    w_menu = init_w(14, 14, 4 + zeroy, 40 + zerox);
    w_inventory = init_w(7, 54, 19 + zeroy, zerox);
    w_debug_bar = init_w(35, 40, zeroy, 57 + zerox);

    p_status_bar = new_panel(w_status_bar);
    p_map = new_panel(w_map);
    p_menu = new_panel(w_menu);
    p_inventory = new_panel(w_inventory);
    p_avatar = new_panel(w_avatar);
    p_debug_bar = new_panel(w_debug_bar);

    top_panel(p_menu);
    update_panels();
    doupdate();

    status("Nice");
    game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
    print_inventory_();
    print_avatar_(health_);
}


GameGUI::~GameGUI() {
    status("Press any key to exit...");
    getch();
    del_panel(p_status_bar);
    del_panel(p_map);
    del_panel(p_menu);
    del_panel(p_inventory);
    del_panel(p_avatar);
    del_panel(p_debug_bar);
    delwin(w_status_bar);
    delwin(w_map);
    delwin(w_menu);
    delwin(w_inventory);
    delwin(w_avatar);
    delwin(w_debug_bar);
}


// Запрос хода у игрока и возврат структуры с
// данными о нем для дальнейшей отправки на сервер
Turn GameGUI::send_request() {
    Turn turn_info;
    Command command;
    //bool end_input = false;
    status("Your turn!!!");
    while (true) {
        while (true) {
            command = move_cursor_(str_command_);
            if (command.type != TurnTypes::END || end_game_())
                break;
        }
        turn_info.m = cur_m_ + command.d_m;
        turn_info.n = cur_n_ + command.d_n;
        turn_info.turn = command.type;
        if (!command.is_directed) {
            //debug(turn_info);
            return turn_info;
        }
        status(command.name + " to...");
        command = move_cursor_(str_move_);
        if (command.type != TurnTypes::END)
            break;
    }
    turn_info.m = cur_m_ + command.d_m;
    turn_info.n = cur_n_ + command.d_n;
    //debug(turn_info);
    return turn_info;
}


GameGUI::Command GameGUI::move_cursor_(std::vector<Command> comds) {
    cursor_ = 0;
    int key;
    while (true) {
        print_menu_(comds);
        switch(key = getch()) {
            case KEY_UP:
            case KEY_LEFT: {
                if (cursor_-- == 0) cursor_ = comds.size() - 1;
                break;
            }
            case KEY_DOWN:
            case KEY_RIGHT: {
                if (++cursor_ == comds.size()) cursor_ = 0;
                break;
            }
            case 10:
                return comds[cursor_];
            case KEY_BACKSPACE: {
                return {
                        " ", ' ', false,
                        TurnTypes::END,
                        0, 0
                };
            }
            default: {
                for (uint32_t i = 0; i < comds.size(); i++)
                    if (key == comds[i].hot_key)
                        return comds[i];
            }
        }
    }
}


// Получение и обработка ответа от сервера.
// Если пришла структура с ошибкой (turn.type_of_error != NONE),
// то возвращает 1, иначе - 0
bool GameGUI::get_response(ShowTurn turn) {
    //debug(turn);
    if (turn.error == ErrorTypes::NO_ERR_IN_MSG) {
        if (health_ == 0) {
            has_treasure_ = false;
            bombs_ = aids_ = ammo_ = concrete_ = 0;
        }
        switch(turn.turn) {
            case TurnTypes::WATCH: {
                do_watch_(turn);
                break;
            }
            case TurnTypes::GO: {
                do_go_(turn);
                break;
            }
            case TurnTypes::KNIFE: {
                do_knife_(turn);
                break;
            }
            case TurnTypes::SHOOT: {
                do_shoot_(turn);
                break;
            }
            case TurnTypes::BOMB: {
                do_bomb_(turn);
                break;
            }
            case TurnTypes::CONCRETE: {
                do_concrete_(turn);
                break;
            }
            case TurnTypes::AID: {
                do_aid_(turn);
                break;
            }
            default:
                status("WTF???");
        }
        status(turn.turn);
        status("to", false);
        status(turn.m, false);
        status(turn.n, false);
        game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
        print_inventory_();
        print_avatar_(health_);
        return true;
    }
    status(turn.error);
    return false;
}


bool GameGUI::get_response(ShowOtherTurn turn) {
    //debug(turn);
    if (health_ == 0) {
        has_treasure_ = false;
        bombs_ = aids_ = ammo_ = concrete_ = 0;
    }
    switch(turn.turn) {
        case TurnTypes::GO: {
            do_go_(turn);
            break;
        }
        case TurnTypes::KNIFE: {
            do_knife_(turn);
            break;
        }
        case TurnTypes::BOMB: {
            do_bomb_(turn);
            break;
        }
        case TurnTypes::CONCRETE: {
            do_concrete_(turn);
            break;
        }
    }
    status("Wait your turn...");
    game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
    print_inventory_();
    print_avatar_(health_);
    return true;
}


void GameGUI::do_watch_(ShowTurn turn) {
    game_map_[turn.m][turn.n].first = turn.field;
    game_map_[turn.m][turn.n].second = turn.treasure;
}


void GameGUI::do_go_(ShowTurn turn) {
    bombs_ = turn.bombs;
    ammo_ = turn.ammo;
    aids_ = turn.aids;
    concrete_ = turn.concrete;
    game_map_[turn.m][turn.n].first = turn.field;
    game_map_[turn.m][turn.n].second = turn.treasure;
    switch (turn.field) {
        case FieldTypes::WALL:
        case FieldTypes::CONCRETE:
            break;
        case FieldTypes::ROAD: {
            if (!(turn.m == 0 || turn.m == 11
                || turn.n == 0 || turn.n == 11)) {
                cur_m_ = turn.m;
                cur_n_ = turn.n;
            }
            break;
        }
        case FieldTypes::RIVER: {
            status("Swim");
            for (auto water : turn.river_coordinates) {
                status(".", false);
                game_map_[water.first][water.second].first = FieldTypes::RIVER;
                game_map_[water.first][water.second].second = false;
                cur_m_ = water.first;
                cur_n_ = water.second;
                game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            break;
        }
        case FieldTypes::METRO: {
            cur_m_ = turn.m;
            cur_n_ = turn.n;
            game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            game_map_[turn.metro_m][turn.metro_n].first = turn.field;
            game_map_[turn.metro_m][turn.metro_n].second = turn.treasure;
            if (health_ > 0)
                game_map_[turn.metro_m][turn.metro_n].second = false;
            cur_m_ = turn.metro_m;
            cur_n_ = turn.metro_n;
            break;
        }
        case FieldTypes::ARMOURY: {
            cur_m_ = turn.m;
            cur_n_ = turn.n;
            break;
        }
        case FieldTypes::HOSPITAL: {
            cur_m_ = turn.m;
            cur_n_ = turn.n;
            if (health_ < turn.health) {
                print_avatar_(turn.health);
                game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            break;
        }
        case FieldTypes::BOMB: {
            cur_m_ = turn.m;
            cur_n_ = turn.n;
            //status(turn.health);
            //getch();
            if (health_ > turn.health) {
                game_map_.paint(w_map, cur_m_, cur_n_, false, other_players);
                print_avatar_(turn.health);
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                health_ = turn.health;
                game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
                print_avatar_(health_);
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                game_map_[turn.m][turn.n].first = FieldTypes::ROAD;
            }
            break;
        }
        default:
            status("4e?");
    }
    health_ = turn.health;
    if (health_ > 0) {
        game_map_[turn.m][turn.n].second = false;
        has_treasure_ = has_treasure_ || turn.treasure;
    }
}


void GameGUI::do_knife_(ShowTurn turn) {
    game_map_[turn.m][turn.n].first = turn.field;
    game_map_[turn.m][turn.n].second = turn.treasure;
}


void GameGUI::do_shoot_(ShowTurn turn) {}


void GameGUI::do_bomb_(ShowTurn turn) {
    bombs_--;
    game_map_[turn.m][turn.n].first = turn.field;
    game_map_[turn.m][turn.n].second =
            game_map_[turn.m][turn.n].second || turn.treasure;
}


void GameGUI::do_concrete_(ShowTurn turn) {
    concrete_--;
    game_map_[turn.m][turn.n].first = turn.field;
    game_map_[turn.m][turn.n].second =
            game_map_[turn.m][turn.n].second || turn.treasure;
}


void GameGUI::do_aid_(ShowTurn turn) {
    int32_t old_h = health_;
    health_ = turn.health;
    aids_--;
    if (old_h < health_) {
        print_avatar_(turn.health);
        game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}


// MEGAGOVNOKOD
void GameGUI::do_go_(ShowOtherTurn turn) {
    bool flag = false;
    // status(turn.current_treasure);
    //status(turn.visible_to, false);
    //getch();
    if (turn.visible_from && !turn.previous_treasure)
        game_map_[turn.previous_m][turn.previous_n].second = false;
    if (turn.visible_from && turn.previous_treasure)
        game_map_[turn.previous_m][turn.previous_n].second = true;
    if (turn.visible_to && !turn.current_treasure)
        game_map_[turn.current_m][turn.current_n].second = false;
    if (turn.visible_to && turn.current_treasure)
        game_map_[turn.current_m][turn.current_n].second = true;
    if (turn.treasure)
        status("Treasure ");
    if (turn.visible_to) {
        switch (turn.current_field) {
            case FieldTypes::RIVER: {
                status("Swim");
                int32_t m, n;
                for (auto water : turn.river_coordinates) {
                    m = water.first;
                    n = water.second;
                    status(".", false);
                    other_players[turn.id] = {m, n,
                                              turn.current_player ==
                                              PlayerTypes::DEAD_PLAYER};
                    game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
                //status(m);
                //status(n, false);
                //getch();
                /*other_players[turn.id] = {m, n,
                                          turn.current_player ==
                                          PlayerTypes::DEAD_PLAYER};
                if (turn.current_player != PlayerTypes::NO_PLAYER) {
                    other_players[turn.id] = {turn.current_m,
                                              turn.current_n,
                                              turn.current_player ==
                                              PlayerTypes::DEAD_PLAYER};
                    flag = true;
                    }*/
                flag = true;
                break;
            }
            case FieldTypes::BOMB: {
                game_map_[turn.current_m][turn.current_n].first = FieldTypes::ROAD;
            }
            default: {
                if (turn.current_player != PlayerTypes::NO_PLAYER) {
                    other_players[turn.id] = {turn.current_m,
                                              turn.current_n,
                                              turn.current_player ==
                                                      PlayerTypes::DEAD_PLAYER};
                    flag = true;
                }
            }
        }
    }
    if (turn.visible_from) {
        if (turn.previous_player != PlayerTypes::NO_PLAYER /*&&
                                                             turn.current_field != FieldTypes::RIVER*/) {
            other_players[turn.id] = {turn.previous_m,
                                      turn.previous_n,
                                      turn.previous_player ==
                                      PlayerTypes::DEAD_PLAYER};
            flag = true;
        }
    }
    if (!flag)
        other_players[turn.id] = {-1, -1, false};
}


void GameGUI::do_knife_(ShowOtherTurn turn) {
    if (health_ != turn.health) {
        game_map_.paint(w_map, cur_m_, cur_n_, false, other_players);
        print_avatar_(turn.health);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        health_ = turn.health;
        game_map_.paint(w_map, cur_m_, cur_n_, health_ == 0, other_players);
        print_avatar_(health_);
    }
}


void GameGUI::do_bomb_(ShowOtherTurn turn) {
    if (turn.visible_to) {
        game_map_[turn.current_m][turn.current_n].first = turn.current_field;
        game_map_[turn.current_m][turn.current_n].second =
                game_map_[turn.current_m][turn.current_n].second || turn.treasure;
    }
}


void GameGUI::do_concrete_(ShowOtherTurn turn) {
    if (turn.visible_to) {
        game_map_[turn.current_m][turn.current_n].first = turn.current_field;
        game_map_[turn.current_m][turn.current_n].second =
                game_map_[turn.current_m][turn.current_n].second || turn.treasure;
    }
}



bool GameGUI::end_game_() {
    int maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);
    int zerox = maxx < 56 ? 21 : (maxx - 13) / 2;
    int zeroy = maxy < 27 ? 8 : (maxy - 12) / 2;
    WINDOW* w = init_w(3, 13, zeroy, zerox, 2);
    PANEL* p = new_panel(w);
    top_panel(p);
    update_panels();
    doupdate();
    bool flag = true, cursor = false;
    while (flag) {
        wclear(w);
        if (cursor)
            wattron(w, A_REVERSE);
        mvwprintw(w, 1, 2, "Yes");
        if (cursor)
            wattroff(w, A_REVERSE);
        if (!cursor)
            wattron(w, A_REVERSE);
        mvwprintw(w, 1, 8, "No");
        if (!cursor)
            wattroff(w, A_REVERSE);
        box(w, ACS_VLINE, ACS_HLINE);
        mvwprintw(w, 0, 3, "=Exit?=");
        wrefresh(w);
        switch(getch()) {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                cursor = !cursor;
                break;
            case 10:
                flag = false;
                break;
            case 'y':
                cursor = true;
                flag = false;
                break;
            case 'n':
                cursor = false;
                flag = false;
        }
    }
    del_panel(p);
    delwin(w);
    update_panels();
    doupdate();
    return cursor;
}
