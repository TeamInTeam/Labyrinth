// Created by Vladislav Melnikov on 10.12.17.

#ifndef LABTECH_MAPCHAR_HPP
#define LABTECH_MAPCHAR_HPP

#include <curses.h>
#include "../../../MACRO_COLOR.hpp"

class MapChar {
private:
    static uint32_t num;
    char chr_;
    short color_;

public:
    MapChar(char chr, short color);

    chtype to_char(bool ghost, bool player, bool other_player,
                   bool other_ghost, bool treasure) const;
};


#endif //LABTECH_MAPCHAR_HPP
