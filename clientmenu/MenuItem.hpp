// Created by Vladislav Melnikov on 07.12.17.

#ifndef LABTECH_MENUITEM_HPP
#define LABTECH_MENUITEM_HPP

#include <iostream>
#include <ncurses.h>
#include "../clientgame/GameGUIw.hpp"


class MENU;

enum class MenuType {
    BUNG = 0,
    LINK = 1,
    START = 2,
    QUIT = 3
};

class MenuItem {
protected:
    std::string name;
    MenuType type;
    MENU** dest;
    MENU* gui;

public:
    MenuItem(MenuType type, std::string str);
    MenuItem(MenuType type, std::string str, MENU** dest, MENU* gui);

    std::string to_string() { return name; };
    bool action();
};


#endif //LABTECH_MENUITEM_HPP
