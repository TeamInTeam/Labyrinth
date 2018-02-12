// Created by Vladislav Melnikov on 03.12.17.

#ifndef LABTECH_GAMEMAP_HPP
#define LABTECH_GAMEMAP_HPP

#include <panel.h>
#include <vector>
#include <map>
#include "../../proto/proto.h"
#include "MapChar/MapChar.hpp"

struct OtherPlayer {
    int32_t m, n;
    bool ghost;
};

class ClientMap {
private:
    int32_t SIZE_MAP;
    std::pair<FieldTypes, bool>** fields_;
    const MapChar char_field_[9] = {
            MapChar(' ', COLOR_WHITE),
            MapChar('#', COLOR_MAGENTA),
            MapChar('~', COLOR_BLUE),
            MapChar('M', COLOR_RED),
            MapChar('A', COLOR_RED),
            MapChar('+', COLOR_GREEN),
            MapChar('.', COLOR_WHITE),
            MapChar('&', COLOR_MAGENTA),
            MapChar('*', COLOR_WHITE),
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

    void paint(WINDOW *w, int32_t cur_m, int32_t cur_n, bool ghost,
               std::map<int32_t, OtherPlayer> other_players);
};

#endif //LABTECH_GAMEMAP_HPP
