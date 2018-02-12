// Created by Vladislav Melnikov on 16.12.17.

#include <cassert>
#include "GameGUI.hpp"


void GameGUI::status(std::string str, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 21, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wprintw(w_status_bar, str.c_str());
    waddch(w_status_bar, ' ');
    wrefresh(w_status_bar);
}


void GameGUI::status(int32_t num, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 21, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wprintw(w_status_bar, "%d ", num);
    wrefresh(w_status_bar);
}


void GameGUI::status(ErrorTypes err, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 21, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wattron(w_status_bar, COLOR_PAIR(3));
    wprintw(w_status_bar, error_str_(err).c_str());
    wattroff(w_status_bar, COLOR_PAIR(3));
    waddch(w_status_bar, ' ');
    wrefresh(w_status_bar);
}


void GameGUI::status(TurnTypes turn, bool clear) {
    if (clear) {
        wclear(w_status_bar);
        box(w_status_bar, ACS_VLINE, ACS_HLINE);
        mvwprintw(w_status_bar, 0, 21, "=STATUS BAR=");
        wmove(w_status_bar, 1, 1);
    }
    wattron(w_status_bar, COLOR_PAIR(4));
    wprintw(w_status_bar, turn_str_(turn).c_str());
    wattroff(w_status_bar, COLOR_PAIR(4));
    waddch(w_status_bar, ' ');
    wrefresh(w_status_bar);
}


void GameGUI::print_inventory_() {
    wclear(w_inventory);
    int left = 1;
    if (aids_) {
        ascii_aid(w_inventory, 1, left, aids_);
        left += 14;
    }
    if (bombs_) {
        ascii_bomb(w_inventory, 1, left, bombs_);
        left += 14;
    }
    if (concrete_) {
        ascii_wall(w_inventory, 1, left, concrete_);
        left += 14;
    }
    if (ammo_)
        ascii_bullet(w_inventory, 1, left, ammo_);
    box(w_inventory, ACS_VLINE, ACS_HLINE);
    mvwprintw(w_inventory, 0, 21, "=INVENTORY=");
    mvwprintw(w_inventory, 6, 11, "BACKSPASE: exit, ENTER: select");
    wrefresh(w_inventory);
}


void GameGUI::print_avatar_(int32_t new_hp) {
    int32_t healed = new_hp - health_;
    wclear(w_avatar);
    if (healed > 0)
        wattron(w_avatar, PAIR_SUC);
    else if (healed < 0)
        wattron(w_avatar, PAIR_ERR);
    else if (has_treasure_)
        wattron(w_avatar, PAIR_TREASURE);
    mvwprintw(w_avatar, 1, 1, "(\\___/)");
    mvwprintw(w_avatar, 2, 1, "(='.'=)");
    mvwprintw(w_avatar, 3, 1, "(\")_(\")");
    if (healed > 0)
        wattroff(w_avatar, PAIR_SUC);
    else if (healed < 0)
        wattroff(w_avatar, PAIR_ERR);
    else if (has_treasure_)
        wattroff(w_avatar, PAIR_TREASURE);
    wmove(w_avatar, 4, 1);
    for (size_t i = 0; i < 7; i++)
        waddch(w_avatar, ACS_HLINE);
    wmove(w_avatar, 5, 1);
    int32_t c1 = health_;
    int32_t c2 = new_hp;
    if (healed < 0)
        std::swap(c1, c2);
    assert((c1 >= 0) && (c1 <= c2));
    assert(c2 <= 7);
    wattron(w_avatar, PAIR_FULL_HP);
    for (int32_t i = 0; i <= 7; i++) {
        if (i == c1) {
            wattroff(w_avatar, PAIR_FULL_HP);
            wattron(w_avatar, PAIR_INV);
        }
        if (i == c2)
            wattroff(w_avatar, PAIR_INV);
        switch(i) {
            case 2:
                waddch(w_avatar, 'H');
                break;
            case 3:
                waddch(w_avatar, 'P');
                break;
            default:
                waddch(w_avatar, ' ');
        }
    }
    box(w_avatar, ACS_VLINE, ACS_HLINE);
    mvwaddch(w_avatar, 4, 0, ACS_LTEE);
    mvwaddch(w_avatar, 4, 8, ACS_RTEE);
    wrefresh(w_avatar);
}


void GameGUI::print_menu_(std::vector<Command> comds) {
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


//void GameGUI::debug(std::string str) {
//    wprintw(w_debug_bar, "%s; ", str.c_str());
//    wrefresh(w_debug_bar);
//}
//
//
//void GameGUI::debug(Turn turn) {
//    //wprintw(w_debug_bar, "Send; ");
//    wprintw(w_debug_bar, "%s to %d,%d; ",
//            turn_str_(turn.turn).c_str(),
//            turn.m, turn.n);
//    wrefresh(w_debug_bar);
//}
//
//
//void GameGUI::debug(ShowTurn turn) {
//    //wprintw(w_debug_bar, "Get; ");
//    wprintw(w_debug_bar, "%s to %d,%d with %s at %s t:%d b:%d am:%d ai:%d c:%d hp:%d; ",
//            turn_str_(turn.turn).c_str(), turn.m, turn.n,
//            error_str_(turn.error).c_str(),
//            field_str_(turn.field).c_str(),
//            turn.treasure, turn.bombs, turn.ammo,
//            turn.aids, turn.concrete, turn.health);
//    wrefresh(w_debug_bar);
//}


std::string GameGUI::error_str_(ErrorTypes error) {
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


std::string GameGUI::turn_str_(TurnTypes turn) {
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


std::string GameGUI::field_str_(FieldTypes field) {
    switch(field) {
        case FieldTypes::ROAD:
            return "Road";
        case FieldTypes::WALL:
            return "Wall";
        case FieldTypes::RIVER:
            return "River";
        case FieldTypes::METRO:
            return "Metro";
        case FieldTypes::ARMOURY:
            return "Armoury";
        case FieldTypes::HOSPITAL:
            return "Hospital";
        case FieldTypes::BOMB:
            return "Mine";
        case FieldTypes::CONCRETE:
            return "Concrete";
        default:
            return "WTFEEE";
    }
}
