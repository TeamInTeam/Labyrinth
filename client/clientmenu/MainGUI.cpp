// Created by Vladislav Melnikov on 05.12.17.

#include "MainGUI.hpp"

void init_ncurses() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);

    if (!has_colors()) {
        endwin();
        printf("\nBad colors\n");
        return;
    }
    start_color();

    init_color(COLOR_CYAN, 200, 150, 40);
    init_color(COLOR_GHOST, 300, 225, 60);
    init_color(COLOR_MAGENTA, 600, 600, 600);
    init_color(COLOR_ORANGE, 800, 400, 300);
    init_color(COLOR_GRAYG, 600, 800, 600);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_GREEN);
    //init_pair(6, COLOR_WHITE, COLOR_YELLOW);
    init_pair(7, COLOR_WHITE, COLOR_RED);
    init_pair(8, COLOR_ORANGE, COLOR_BLACK);
    init_pair(9, COLOR_GRAYG, COLOR_BLACK);
    init_pair(10, COLOR_WHITE, COLOR_CYAN);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
}


MainGUI::MainGUI() {
    init_ncurses();

    int maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);
    int zerox = maxx < 56 ? 2 : (maxx - 64) / 2;
    int zeroy = maxy < 27 ? 1 : (maxy - 9) / 2;
    WINDOW* wtitle = newwin(9, 64, zeroy, zerox);
    wbkgdset(wtitle, COLOR_PAIR(1));
    PANEL* ptitle = new_panel(wtitle);
    update_panels();
    doupdate();

    wclear(wtitle);
    mvwprintw(wtitle, 1, 1, "$$______$$$$__$$$$$__$$__$$_$$$$$__$$$$$$_$$__$$_$$$$$$_$$__$$");
    mvwprintw(wtitle, 2, 1, "$$_____$$__$$_$$__$$__$$$$__$$__$$___$$___$$$_$$___$$___$$__$$");
    mvwprintw(wtitle, 3, 1, "$$_____$$$$$$_$$$$$____$$___$$$$$____$$___$$_$$$___$$___$$$$$$");
    mvwprintw(wtitle, 4, 1, "$$_____$$__$$_$$__$$___$$___$$__$$___$$___$$__$$___$$___$$__$$");
    mvwprintw(wtitle, 5, 1, "$$$$$$_$$__$$_$$$$$____$$___$$__$$_$$$$$$_$$__$$___$$___$$__$$");
    mvwprintw(wtitle, 7, 20, "Press any key to begin...");
    box(wtitle, ACS_VLINE, ACS_HLINE);
    wrefresh(wtitle);
    getch();
    del_panel(ptitle);
    delwin(wtitle);

    main_menu_ = MENU({
            new BUNG("Main menu"),
            new START("New game"),
            new BUNG("Log in"),
            new LINK("Settings", &cur_window_, &settings_),
            new QUIT("Quit")
    });

    settings_ = MENU({
            new BUNG("Settings"),
            new BUNG("Invert colors"),
            new LINK("Back", &cur_window_, &main_menu_)
    });

    cur_window_ = &main_menu_;
}


MainGUI::~MainGUI() {
    endwin();
}


// Пока пользователь не выходит из игры,
// даем ему возможность выбирать
void MainGUI::go() {
    while (cur_window_->select());
}