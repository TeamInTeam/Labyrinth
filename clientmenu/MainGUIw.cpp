// Created by Vladislav Melnikov on 05.12.17.

#include "MainGUIw.hpp"

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
    init_color(COLOR_MAGENTA, 600, 600, 600);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);

    init_pair(11, COLOR_MAGENTA, COLOR_CYAN);
    init_pair(12, COLOR_BLUE, COLOR_CYAN);
    init_pair(13, COLOR_RED, COLOR_CYAN);
    init_pair(14, COLOR_GREEN, COLOR_CYAN);
    init_pair(15, COLOR_WHITE, COLOR_CYAN);

    init_pair(21, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(22, COLOR_BLUE, COLOR_YELLOW);
    init_pair(23, COLOR_RED, COLOR_YELLOW);
    init_pair(24, COLOR_GREEN, COLOR_YELLOW);
    init_pair(25, COLOR_WHITE, COLOR_YELLOW);

    bkgd(COLOR_PAIR(1));
}


MainGUIw::MainGUIw() {
    init_ncurses();

    WINDOW* wtitle = newwin(9, 64, 1, 2);
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

    main_menu_.constructor({
                                   MenuItem(MenuType::BUNG, "Main menu", &cur_window_, &main_menu_),
                                   MenuItem(MenuType::START, "New game"),
                                   MenuItem(MenuType::BUNG, "Log in"),
                                   MenuItem(MenuType::LINK, "Settings", &cur_window_, &settings_),
                                   MenuItem(MenuType::QUIT, "Quit")
                           });
    settings_.constructor({
                                  MenuItem(MenuType::BUNG, "Settingsss"),
                                  MenuItem(MenuType::BUNG, "Invert colors"),
                                  MenuItem(MenuType::LINK, "Back", &cur_window_, &main_menu_)
                          });


    cur_window_ = &main_menu_;
}


MainGUIw::~MainGUIw() {
    endwin();
}


void MainGUIw::go() {
    while (cur_window_->select());
}