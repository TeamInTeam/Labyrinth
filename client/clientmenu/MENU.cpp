// Created by Vladislav Melnikov on 05.12.17.

#include "MENU.hpp"


MENU::MENU(std::initializer_list<MenuItem*> fields) {
    fields_ = fields;
    int maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);
    int zerox = maxx < 56 ? 2 : (maxx - 42) / 2;
    int zeroy = maxy < 27 ? 1 : (maxy - 22) / 2;
    w = newwin(22, 42, zeroy, zerox);
    wbkgdset(w, COLOR_PAIR(1));
    p = new_panel(w);
    update_panels();
    doupdate();
}


MENU::~MENU() {
    del_panel(p);
    delwin(w);
    for (uint32_t i = 0; i < fields_.size(); i++)
        delete fields_[i];
}


MENU& MENU::operator=(MENU &&men) noexcept {
    fields_ = std::move(men.fields_);
    w = men.w;
    p = men.p;
    cursor_ = men.cursor_;
    men.w = nullptr;
    men.p = nullptr;
    return *this;
}


// Вывод менюшки на экран
void MENU::print() {
    wclear(w);
    for (uint32_t i = 1; i < fields_.size(); i++) {
        wmove(w, i + 5, 21 - fields_[i]->to_string().size() / 2);
        if (i == cursor_)
            wattron(w, A_REVERSE);
        wprintw(w, fields_[i]->to_string().c_str());
        if (i == cursor_)
            wattroff(w, A_REVERSE);
    }
    box(w, ACS_VLINE, ACS_HLINE);
    mvwprintw(w, 0, 21 - fields_[0]->to_string().size() / 2, fields_[0]->to_string().c_str());
    mvwprintw(w, 21, 28, "ENTER: select");
    wrefresh(w);
}


// Контроллер для перемещения курсора по меню.
// Возвращает 0, если нужно выйти из игры, 1 - иначе.
bool MENU::select() {
    show_panel(p);
    update_panels();
    doupdate();
    cursor_ = 1;
    bool flag = true;
    while (flag) {
        print();
        switch(getch()) {
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
        }
    }
    hide_panel(p);
    return fields_[cursor_]->action();
}