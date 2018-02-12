// Created by Vladislav Melnikov on 09.12.17.

#ifndef LABTECH_BUNG_HPP
#define LABTECH_BUNG_HPP

#include "../MenuItem.hpp"

class BUNG: public MenuItem {
public:
    BUNG(std::string str);
    bool action();
};


#endif //LABTECH_BUNG_HPP
