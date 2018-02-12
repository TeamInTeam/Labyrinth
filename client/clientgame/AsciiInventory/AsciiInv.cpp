// Created by Vladislav Melnikov on 16.12.17.

#include "AsciiInv.hpp"


void ascii_bullet(WINDOW* w, int m, int n, int count) {
    wattron(w, PAIR_ERR);
    wmove(w, m, n);
    waddch(w, ACS_ULCORNER);
    for (int i = 0; i < 8; i++)
        waddch(w, ACS_HLINE);
    waddch(w, ACS_URCORNER);
    wmove(w, m + 1, n);
    waddch(w, ACS_VLINE);
    wprintw(w, "KAMA THE");
    waddch(w, ACS_VLINE);
    wmove(w, m + 2, n);
    waddch(w, ACS_VLINE);
    wprintw(w, "BULLET");
    waddch(w, ACS_ULCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_RTEE);
    wmove(w, m + 3, n);
    waddch(w, ACS_VLINE);
    for (int i = 0; i < 6; i++)
        waddch(w, ' ');
    waddch(w, ACS_VLINE);
    wprintw(w, "%d", count);
    waddch(w, ACS_VLINE);
    wmove(w, m + 4, n);
    waddch(w, ACS_LLCORNER);
    for (int i = 0; i < 6; i++)
        waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_LRCORNER);
    wattroff(w, PAIR_ERR);
}


void ascii_wall(WINDOW* w, int m, int n, int count) {
    wattron(w, PAIR_WALL);
    wmove(w, m, n);
    waddch(w, ACS_ULCORNER);
    for (int i = 0; i < 3; i++)
        waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    for (int i = 0; i < 3; i++)
        waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_URCORNER);
    wmove(w, m + 1, n);
    waddch(w, ACS_LTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_RTEE);
    wmove(w, m + 2, n);
    waddch(w, ACS_LTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_RTEE);
    wmove(w, m + 3, n);
    waddch(w, ACS_LTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_TTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_RTEE);
    wprintw(w, "%d", count);
    waddch(w, ACS_VLINE);
    wmove(w, m + 4, n);
    waddch(w, ACS_LLCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    for (int i = 0; i < 3; i++)
        waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_LRCORNER);
    wattroff(w, PAIR_WALL);
}


void ascii_aid(WINDOW* w, int m, int n, int count) {
    wattron(w, PAIR_SUC);
    wmove(w, m, n);
    waddch(w, ACS_ULCORNER);
    for (int i = 0; i < 9; i++)
        waddch(w, ACS_HLINE);
    waddch(w, ACS_URCORNER);
    wmove(w, m + 1, n);
    waddch(w, ACS_VLINE);
    wprintw(w, "   ");
    waddch(w, ACS_ULCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_URCORNER);
    wprintw(w, "   ");
    waddch(w, ACS_VLINE);
    wmove(w, m + 2, n);
    waddch(w, ACS_VLINE);
    waddch(w, ' ');
    waddch(w, ACS_ULCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_LRCORNER);
    waddch(w, ' ');
    waddch(w, ACS_LLCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_URCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_RTEE);
    wmove(w, m + 3, n);
    waddch(w, ACS_VLINE);
    waddch(w, ' ');
    waddch(w, ACS_LLCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_URCORNER);
    waddch(w, ' ');
    waddch(w, ACS_ULCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_LRCORNER);
    wprintw(w, "%d", count);
    waddch(w, ACS_VLINE);
    wmove(w, m + 4, n);
    waddch(w, ACS_LLCORNER);
    for (int i = 0; i < 3; i++)
        waddch(w, ACS_HLINE);
    waddch(w, ACS_LLCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_LRCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_LRCORNER);
    wattroff(w, PAIR_SUC);
}


void ascii_bomb(WINDOW* w, int m, int n, int count) {
    wattron(w, PAIR_BOMB);
    wmove(w, m, n);
    waddch(w, ACS_ULCORNER);
    for (int i = 0; i < 9; i++)
        waddch(w, ACS_TTEE);
    waddch(w, ACS_URCORNER);
    wmove(w, m + 1, n);
    waddch(w, ACS_LTEE);
    for (int i = 0; i < 9; i++)
        waddch(w, ACS_BTEE);
    waddch(w, ACS_RTEE);
    wmove(w, m + 2, n);
    waddch(w, ACS_VLINE);
    waddch(w, ' ');
    waddch(w, ' ');
    waddch(w, 'T');
    waddch(w, ' ');
    waddch(w, 'N');
    waddch(w, ' ');
    waddch(w, 'T');
    waddch(w, ACS_ULCORNER);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_RTEE);
    wmove(w, m + 3, n);
    waddch(w, ACS_LTEE);
    for (int i = 0; i < 7; i++)
        waddch(w, ACS_TTEE);
    waddch(w, ACS_RTEE);
    wprintw(w, "%d", count);
    waddch(w, ACS_VLINE);
    wmove(w, m + 4, n);
    waddch(w, ACS_LLCORNER);
    for (int i = 0; i < 8; i++)
        waddch(w, ACS_BTEE);
    waddch(w, ACS_HLINE);
    waddch(w, ACS_LRCORNER);
    wattroff(w, PAIR_BOMB);
}