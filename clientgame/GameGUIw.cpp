// Created by Vladislav Melnikov on 03.12.17.

#include "GameGUIw.hpp"
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


GameGUIw::GameGUIw(ShowTurn new_game) :
game_map_(new_game.size_m){
    cur_m_ = new_game.m; cur_n_ = new_game.n;
    health_ = new_game.health;
    game_map_[cur_m_][cur_n_].first = new_game.type_of_field;
    has_treasure_ = false;
    cursor_ = 0;

    w_status_bar = init_w(3, 42, 1, 2);
    w_map = init_w(14, 25, 5, 2);
    w_menu = init_w(14, 14, 5, 30);
    w_inventory = init_w(3, 42, 20, 2);

    p_status_bar = new_panel(w_status_bar);
    p_map = new_panel(w_map);
    p_menu = new_panel(w_menu);
    p_inventory = new_panel(w_inventory);

    top_panel(p_menu);
    update_panels();
    doupdate();

    status("Nice");
    game_map_.paint(w_map, cur_m_, cur_n_);
    print_inventory_();
}


GameGUIw::~GameGUIw() {
    status("Press any key to exit...");
    getch();
    del_panel(p_status_bar);
    del_panel(p_map);
    del_panel(p_menu);
    del_panel(p_inventory);
    delwin(w_status_bar);
    delwin(w_map);
    delwin(w_menu);
    delwin(w_inventory);
    endwin();
}


// !!!!!!!!!!!!ПЕРЕПИСАТЬ!!!!!!!!!!!!
// Запрос хода у игрока и возврат структуры с
// данными о нем для дальнейшей отправки на сервер
Turn GameGUIw::send_request() {
    Turn turn_info;
    TurnTypes command;
    bool moved;
    while (true) {
        moved = move_cursor_(str_command_);
        command = from_type<TurnTypes>(cursor_ + 1);
        if (moved) {
            cursor_ -= 7;
            command = TurnTypes::GO;
            break;
        }
        if (command != TurnTypes::END || end_game_())
            break;
    }
    turn_info.type_of_turn = command;
    int32_t new_m = cur_m_, new_n = cur_n_;
    if (is_directed_(command)) {
        status(str_command_[cursor_].name + " to...");
        while (!moved) {
            move_cursor_(str_move_);
            if (cursor_ != to_type(TurnTypes::END) - 1) {
                break;
            } else if (end_game_()) {
                turn_info.type_of_turn = TurnTypes::END;
                break;
            }
        }
        switch (cursor_) {
            case 0:
                new_m--;
                break;
            case 2:
                new_m++;
                break;
            case 1:
                new_n--;
                show_panel(p_inventory);
                break;
            case 3:
                new_n++;
                break;
            default:
                status("???");
        }
    }
    turn_info.m = new_m;
    turn_info.n = new_n;
    return turn_info;
}


// Получение и обработка ответа от сервера.
// Если пришла структура с ошибкой (turn.type_of_error != NONE),
// то возвращает 1, иначе - 0
bool GameGUIw::get_response(ShowTurn turn) {
    if (turn.type_of_error == ErrorTypes::NO_ERROR) {
        switch(turn.type_of_turn) {
            case TurnTypes::WATCH:
                game_map_[turn.m][turn.n].first = turn.type_of_field;
                game_map_[turn.m][turn.n].second = turn.get_treasure;
                break;
            case TurnTypes::GO:
                game_map_[turn.m][turn.n].first = turn.type_of_field;
                game_map_[turn.m][turn.n].second = false;
                switch(turn.type_of_field) {
                    case FieldTypes::WALL:
                    case FieldTypes::CONCRETE:
                        break;
                    case FieldTypes::ROAD: {
                        has_treasure_ = has_treasure_ || turn.get_treasure;
                        cur_m_ = turn.m;
                        cur_n_ = turn.n;
                        break;
                    }
                    case FieldTypes::RIVER: {
                        status("Swim");
                        for (auto water : turn.river_coordinates) {
                            status(".", false);
                            game_map_.paint(w_map, cur_m_, cur_n_);
                            game_map_[water.first][water.second].first = FieldTypes::RIVER;
                            game_map_[water.first][water.second].second = false;
                            cur_m_ = water.first;
                            cur_n_ = water.second;
                            std::this_thread::sleep_for(std::chrono::milliseconds(300));
                        }
                        break;
                    }
                    case FieldTypes::METRO: {
                        has_treasure_ = has_treasure_ || turn.get_treasure;
                        game_map_[turn.metro_m][turn.metro_n].first = turn.type_of_field;
                        game_map_[turn.metro_m][turn.metro_n].second = false;
                        cur_m_ = turn.metro_m;
                        cur_n_ = turn.metro_n;
                        break;
                    }
                }
                break;
            default:
                status("WTF???");
        }
        status(turn.type_of_turn);
//        if (turn.was_moved) {
//            cur_m_ = turn.m;
//            cur_n_ = turn.n;
//            if (!has_treasure_)
//                has_treasure_ = turn.get_treasure;
//        }
        status("to", false);
        status(turn.m, false);
        status(turn.n, false);
        game_map_.paint(w_map, cur_m_, cur_n_);
        print_inventory_();
        return true;
    }
    statuse(turn.type_of_error);
    status("reenter, please", false);
    return false;
}


void GameGUIw::status(std::string str, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 15, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wprintw(w_status_bar, str.c_str());
    waddch(w_status_bar, ' ');
    wrefresh(w_status_bar);
}


void GameGUIw::status(int32_t num, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 15, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wprintw(w_status_bar, "%d ", num);
    wrefresh(w_status_bar);
}


void GameGUIw::statuse(ErrorTypes err, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 15, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wattron(w_status_bar, COLOR_PAIR(3));
    wprintw(w_status_bar, error_str_(err).c_str());
    wattroff(w_status_bar, COLOR_PAIR(3));
    waddch(w_status_bar, ' ');
    wrefresh(w_status_bar);
}


void GameGUIw::status(TurnTypes turn, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 15, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wattron(w_status_bar, COLOR_PAIR(4));
    wprintw(w_status_bar, turn_str_(turn).c_str());
    wattroff(w_status_bar, COLOR_PAIR(4));
    waddch(w_status_bar, ' ');
    wrefresh(w_status_bar);
}


bool GameGUIw::is_directed_(TurnTypes &f) {
    if (to_type(f) < 6)
        return true;
    return false;
}


void GameGUIw::print_inventory_() {
    wclear(w_inventory);
    wmove(w_inventory, 1, 1);
    if (has_treasure_)
        wprintw(w_inventory, "Treasure");
    else
        wprintw(w_inventory, "Empty");
    box(w_inventory, ACS_VLINE, ACS_HLINE);
    mvwprintw(w_inventory, 0, 15, "=INVENTORY=");
    mvwprintw(w_inventory, 2, 11, "BACKSPASE: exit, ENTER: select");
    wrefresh(w_inventory);
}


void GameGUIw::print_menu_(std::vector<Command> comds) {
    wclear(w_menu);
    wmove(w_menu, 1, 0);
    for (uint32_t i = 0; i < comds.size(); i++) {
        waddch(w_menu, ' ');
        if (i == cursor_)
            wattron(w_menu, A_REVERSE);
        wprintw(w_menu, "[%c] %s", comds[i].hot_key - 32, comds[i].name.c_str());
        if (i == cursor_)
            wattroff(w_menu, A_REVERSE);
        waddch(w_menu, '\n');
    }
    waddch(w_menu, ' ');
    box(w_menu, ACS_VLINE, ACS_HLINE);
    mvwprintw(w_menu, 0, 2, "=COMMANDS=");
    wrefresh(w_menu);
}


// !!!!!!!!!!!!ПЕРЕПИСАТЬ!!!!!!!!!!!!
bool GameGUIw::move_cursor_(std::vector<Command> comds) {
    cursor_ = 0;
    int key;
    while (true) {
        print_menu_(comds);
        switch(key = getch()) {
            case KEY_UP:
            case KEY_LEFT:
                if (cursor_-- == 0) cursor_ = comds.size() - 1;
                break;
            case KEY_DOWN:
            case KEY_RIGHT:
                if (++cursor_ == comds.size()) cursor_ = 0;
                break;
            case 10:
                return is_moved_(comds[cursor_]);
            case KEY_BACKSPACE:
                cursor_ = to_type(TurnTypes::END) - 1;
                return false;
            default:
                for (uint32_t i = 0; i < comds.size(); i++)
                    if (key == comds[i].hot_key) {
                        cursor_ = i;
                        return is_moved_(comds[cursor_]);
                    }
        }
    }
}


bool GameGUIw::is_moved_(Command command) {
    return command.hot_key == 'w' || command.hot_key == 'a'
           || command.hot_key == 's' || command.hot_key == 'd';
}


std::string GameGUIw::error_str_(ErrorTypes error) {
    switch(error) {
        case ErrorTypes::NOT_MY_TURN:
            return "Wait for your turn";
        case ErrorTypes::UNKNOWN_COMMAND:
            return "Unknown command";
        case ErrorTypes::INVALID_DISTANCE:
            return "Invalid distance";
        case ErrorTypes::INVALID_DIRECTION:
            return "Select the correct direction";
        case ErrorTypes::NO_AMMO:
            return "You don't have ammo";
        case ErrorTypes::NO_BOMB:
            return "You don't have bomb";
        case ErrorTypes::NO_CONCRETE:
            return "You don't have concrete";
        case ErrorTypes::NO_AID:
            return "You don't have aid";
        default:
            return "WTF???";
    }
}


std::string GameGUIw::turn_str_(TurnTypes turn) {
    switch(turn) {
        case TurnTypes::START:
            return "Start game";
        case TurnTypes::WATCH:
            return "Watch";
        case TurnTypes::GO:
            return "Go";
        case TurnTypes::KNIFE:
            return "Knife";
        case TurnTypes::SHOOT:
            return "Shoot";
        case TurnTypes::BOMB:
            return "Bomb";
        case TurnTypes::CONCRETE:
            return "Concrete";
        case TurnTypes::AID:
            return "Heal";
        case TurnTypes::END:
            return "End";
    }
    return "WTFee??";
}


bool GameGUIw::end_game_() {
    WINDOW* w = init_w(3, 13, 8, 15, 2);
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