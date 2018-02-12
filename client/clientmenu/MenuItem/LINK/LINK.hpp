// Created by Vladislav Melnikov on 09.12.17.

#ifndef LABTECH_LINK_HPP
#define LABTECH_LINK_HPP

#include "../MenuItem.hpp"

class MENU;

class LINK: public MenuItem {
private:
    MENU** dest;
    MENU* gui;
public:
    LINK(std::string str, MENU** dest, MENU* gui);
    bool action();
};


#endif //LABTECH_LINK_HPP
