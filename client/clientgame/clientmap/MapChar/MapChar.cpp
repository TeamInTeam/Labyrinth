// Created by Vladislav Melnikov on 10.12.17.

#include <cassert>
#include "MapChar.hpp"

uint32_t MapChar::num = 11;

MapChar::MapChar(char chr, short color) {
    chr_ = chr;
    color_ = MapChar::num;
    init_pair(MapChar::num++, color, 15);
    init_pair(MapChar::num++, color, COLOR_WHITE);
    init_pair(MapChar::num++, color, COLOR_ORANGE);
    init_pair(MapChar::num++, color, COLOR_GRAYG);
    if (chr_ == ' ')
        init_pair(MapChar::num++, COLOR_YELLOW, COLOR_CYAN);
    else
        init_pair(MapChar::num++, color, COLOR_YELLOW);
    init_pair(MapChar::num++, color, COLOR_CYAN);
}


chtype MapChar::to_char(bool ghost, bool player, bool other_player,
                        bool other_ghost, bool treasure) const {
    if (ghost)
        return chr_ | COLOR_PAIR(color_);
    if (player)
        return chr_ | COLOR_PAIR(color_ + 1);
    if (other_player)
        return chr_ | COLOR_PAIR(color_ + 2);
    if (other_ghost)
        return chr_ | COLOR_PAIR(color_ + 3);
    if (treasure)
        return chr_ == ' ' ?
               ACS_DIAMOND | COLOR_PAIR(color_ + 4) :
               chr_ | COLOR_PAIR(color_ + 4);
    return chr_ | COLOR_PAIR(color_ + 5);
}