// Created by Vladislav Melnikov on 03.12.17.

#ifndef LABTECH_GAMEMAP_HPP
#define LABTECH_GAMEMAP_HPP

#include <panel.h>
#include <vector>
#include "../game.h"

class ClientMap {
private:
    int32_t SIZE_MAP;
    std::pair<FieldTypes, bool>** fields_;
    const chtype char_field_[18] = {
            ' ' | COLOR_PAIR(15),
            '#' | COLOR_PAIR(11),
            '~' | COLOR_PAIR(12),
            'M' | COLOR_PAIR(13),
            'A' | COLOR_PAIR(13),
            '+' | COLOR_PAIR(14),
            '.' | COLOR_PAIR(15),
            '&' | COLOR_PAIR(11),
            '*' | COLOR_PAIR(15),
            ' ' | COLOR_PAIR(25),
            '#' | COLOR_PAIR(21),
            '~' | COLOR_PAIR(22),
            'M' | COLOR_PAIR(23),
            'A' | COLOR_PAIR(23),
            '+' | COLOR_PAIR(24),
            '.' | COLOR_PAIR(25),
            '&' | COLOR_PAIR(21),
            '*' | COLOR_PAIR(25),
    };

public:
    class Row {
    private:
        ClientMap* gmap_;
        int32_t m, size_map;
    public:
        Row(ClientMap *map, int32_t m, int32_t size_m);
        std::pair<FieldTypes, bool>& operator[](int32_t n);
    };

    ClientMap(int32_t SIZE_MAP);
    virtual ~ClientMap();

    ClientMap::Row operator[](int32_t m);

    void paint(WINDOW *w, int32_t cur_m, int32_t cur_n);
};

#endif //LABTECH_GAMEMAP_HPP
