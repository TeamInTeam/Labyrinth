// Created by Vladislav Melnikov on 05.12.17.

#include "MENU.hpp"

void MENU::constructor(std::initializer_list<MenuItem> fields) {
    fields_ = fields;
    w = newwin(22, 42, 1, 2);
    wbkgdset(w, COLOR_PAIR(1));
    p = new_panel(w);
    update_panels();
    doupdate();
}

MENU::~MENU() {
    del_panel(p);
    delwin(w);
}

void MENU::print() {
    wclear(w);
    for (uint32_t i = 1; i < fields_.size(); i++) {
        wmove(w, i + 5, 21 - fields_[i].to_string().size() / 2);
        if (i == cursor_)
            wattron(w, A_REVERSE);
        wprintw(w, fields_[i].to_string().c_str());
        if (i == cursor_)
            wattroff(w, A_REVERSE);
    }
    box(w, ACS_VLINE, ACS_HLINE);
    mvwprintw(w, 0, 21 - fields_[0].to_string().size() / 2, fields_[0].to_string().c_str());
    mvwprintw(w, 21, 28, "ENTER: select");
    wrefresh(w);
}

bool MENU::select() {
    show_panel(p);
    update_panels();
    doupdate();
    cursor_ = 1;
    bool flag = true;
    int key;
    while (flag) {
        print();
        switch(key = getch()) {
            case KEY_UP:
            case KEY_LEFT:
                if (--cursor_ == 0) cursor_ = fields_.size() - 1;
                break;
            case KEY_DOWN:
            case KEY_RIGHT:
                if (++cursor_ == fields_.size()) cursor_ = 1;
                break;
            case 10:
                flag = false;
            default:
                if (key <= '9' && key >= '1') {
                    uint32_t number = key - '1';
                    if (number < fields_.size()) {
                        cursor_ = number;
                        flag = false;
                    }
                }
        }
    }
    hide_panel(p);
    return fields_[cursor_].action();
}