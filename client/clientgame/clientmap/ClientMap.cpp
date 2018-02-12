// Created by Vladislav Melnikov on 03.12.17.

#include <cassert>
#include "ClientMap.hpp"

template <class E>
constexpr auto to_type(E enumrator) noexcept {
    return static_cast<std::underlying_type_t<E>>(enumrator);
}

ClientMap::ClientMap(int32_t SIZE_MAP) {
    this->SIZE_MAP = SIZE_MAP;
    fields_ = new std::pair<FieldTypes, bool>*[SIZE_MAP];
    for (int32_t i = 0; i < SIZE_MAP; i++) {
        fields_[i] = new std::pair<FieldTypes, bool>[SIZE_MAP];
        for (int32_t j = 0; j < SIZE_MAP; j++) {
            fields_[i][j].first = FieldTypes::FOG;
            fields_[i][j].second = false;
        }
    }
}

ClientMap::~ClientMap() {
    for (int32_t i = 0; i < SIZE_MAP; i++)
        delete fields_[i];
    delete fields_;
}

ClientMap::Row::Row(ClientMap *map, int32_t m, int32_t size_m) {
    size_map = size_m;
    assert(m < size_map);
    gmap_ = map;
    this->m = m;
}

ClientMap::Row ClientMap::operator[](int32_t m) {
    assert(m < SIZE_MAP);
    return Row(this, m, SIZE_MAP);
}

std::pair<FieldTypes, bool>& ClientMap::Row::operator[](int32_t n) {
    assert(n < size_map);
    return gmap_->fields_[m][n];
}

void ClientMap::paint(WINDOW *w, int32_t cur_m, int32_t cur_n, bool ghost_,
                      std::map<int32_t, OtherPlayer> other_players) {
    wclear(w);
    for (int32_t i = 0; i < SIZE_MAP; i++) {
        wattron(w, COLOR_PAIR(10));
        wmove(w, i + 1, 0);
        for (int32_t j = 0; j < SIZE_MAP; j++) {
            waddch(w, ' ');
            bool player = cur_m == i && cur_n == j;
            bool ghost = ghost_ && player;
            bool treasure = fields_[i][j].second;
            bool oth_p = false, oth_g = false;
            for (auto ot : other_players) {
                if (ot.second.m == i && ot.second.n == j) {
                    if (ot.second.ghost)
                        oth_g = true;
                    else
                        oth_p = true;
                }
            }
            waddch(w, char_field_[to_type(fields_[i][j].first)]
                    .to_char(ghost, player, oth_p, oth_g, treasure));
        }
        wattroff(w, COLOR_PAIR(10));
    }
    box(w, ACS_VLINE, ACS_HLINE);
    mvwprintw(w, 0, 10, "=MAP=");
    wrefresh(w);
}