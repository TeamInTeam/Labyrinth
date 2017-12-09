// Created by Vladislav Melnikov on 05.12.17.

#ifndef LABTECH_MENU_HPP
#define LABTECH_MENU_HPP

#include <iostream>
#include <vector>
#include <panel.h>
#include "MenuItem.hpp"

class MENU {
private:
    WINDOW* w;
    PANEL* p;
    std::vector<MenuItem> fields_;
    void print();

    uint32_t cursor_;

public:
    MENU() = default;
    void constructor(std::initializer_list<MenuItem> fields);
    virtual ~MENU();

    bool select();
    friend class MenuItem;
};


#endif //LABTECH_MENU_HPP
