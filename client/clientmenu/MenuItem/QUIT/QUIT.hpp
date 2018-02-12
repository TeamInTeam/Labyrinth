// Created by Vladislav Melnikov on 09.12.17.

#ifndef LABTECH_QUIT_HPP
#define LABTECH_QUIT_HPP

#include "../MenuItem.hpp"

class QUIT: public MenuItem {
public:
    QUIT(std::string str);
    bool action();
};


#endif //LABTECH_QUIT_HPP
