// Created by Vladislav Melnikov on 09.12.17.
//#include <boost/asio.hpp>
//#include "../../../network/labyrint_message.hpp"

#include "START.hpp"

START::START(std::string str) : MenuItem(str) {}

// MEGAGOVNOKOD
bool START::action() {
    int maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);
    int zerox = maxx < 56 ? 21 : (maxx - 13) / 2;
    int zeroy = maxy < 27 ? 8 : (maxy - 12) / 2;
    WINDOW* w = newwin(3, 18, zeroy, zerox);
    wbkgdset(w, PAIR_INV);
    PANEL* p = new_panel(w);
    top_panel(p);
    update_panels();
    doupdate();
    wclear(w);
    box(w, ACS_VLINE, ACS_HLINE);
    mvwprintw(w, 1, 1, "1)New 2)Join");
    wrefresh(w);
    int ch = getch();
    Info info;



    switch (ch) {
        case '1': {
            wclear(w);
            box(w, ACS_VLINE, ACS_HLINE);
            mvwprintw(w, 0, 1, "Enter name room:");
            wrefresh(w);
            std::string room = "";
            wmove(w, 1, 1);
            while ((ch = getch()) != 10) {
                room += ch;
                waddch(w, ch);
                wrefresh(w);
            }
            info.name = room;
            info.flag = true;
            wclear(w);
            box(w, ACS_VLINE, ACS_HLINE);
            mvwprintw(w, 0, 0, "Number of players:");
            wrefresh(w);
            info.count = getch() - '0';
            break;
        }
        default:
            info.flag = false;
    }

    del_panel(p);
    delwin(w);
    update_panels();
    doupdate();
    start_game(info);
    return true;
}
