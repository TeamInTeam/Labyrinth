// Created by Vladislav Melnikov on 09.12.17.

#include "QUIT.hpp"


QUIT::QUIT(std::string str) : MenuItem(str) {}

bool QUIT::action() {
    return false;
}