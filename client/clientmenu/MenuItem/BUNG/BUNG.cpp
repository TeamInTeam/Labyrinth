// Created by Vladislav Melnikov on 09.12.17.

#include "BUNG.hpp"


BUNG::BUNG(std::string str) : MenuItem(str) {}


bool BUNG::action() {
    return true;
}