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

void ClientMap::paint(WINDOW *w, int32_t cur_m, int32_t cur_n) {
    wclear(w);
    for (int32_t i = 0; i < SIZE_MAP; i++) {
        wattron(w, COLOR_PAIR(15));
        wmove(w, i + 1, 0);
        for (int32_t j = 0; j < SIZE_MAP; j++) {
            waddch(w, ' ');
            if (i == cur_m && j == cur_n)
                wattron(w, A_REVERSE);
            waddch(w, char_field_[to_type(fields_[i][j].first) +
                    fields_[i][j].second * 9]);
            if (i == cur_m && j == cur_n)
                wattroff(w, A_REVERSE);
        }
        wattroff(w, COLOR_PAIR(15));
    }
    box(w, ACS_VLINE, ACS_HLINE);
    mvwprintw(w, 0, 10, "=MAP=");
    wrefresh(w);
}