// Created by Vladislav Melnikov on 09.12.17.

#ifndef LABTECH_START_HPP
#define LABTECH_START_HPP

#include "../MenuItem.hpp"
#include "../../../clientgame/play_game.hpp"

class START: public MenuItem {
public:
    START(std::string str);
    bool action();
};


#endif //LABTECH_START_HPP
