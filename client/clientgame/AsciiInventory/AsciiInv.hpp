// Created by Vladislav Melnikov on 16.12.17.

#ifndef LABTECH_ASCIIINV_HPP
#define LABTECH_ASCIIINV_HPP


#include <curses.h>
#include "../../MACRO_COLOR.hpp"


void ascii_bullet(WINDOW* w, int m, int n, int count);

void ascii_wall(WINDOW* w, int m, int n, int count);

void ascii_aid(WINDOW* w, int m, int n, int count);

void ascii_bomb(WINDOW* w, int m, int n, int count);


#endif //LABTECH_ASCIIINV_HPP
