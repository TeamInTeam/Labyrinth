// Created by Vladislav Melnikov on 09.12.17.

#include "LINK.hpp"


LINK::LINK(std::string str, MENU **dest, MENU *gui) : MenuItem(str) {
    this->dest = dest;
    this->gui = gui;
}


bool LINK::action() {
    *dest = gui;
    return true;
}