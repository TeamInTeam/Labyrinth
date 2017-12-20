/* Copyright (C) 2017 Mikhail Masyagin */

#include <cinttypes>
#include <string>
#include <algorithm>
#include "activeplayer.h"

ActivePlayer::ActivePlayer(int32_t id, std::string name) :
    id(id),
    name(name),
    m(-1),
    n(-1),
    health(7),
    treasure(false),
    bombs(0),
    ammo(0),
    concrete(0),
    aids(0) {}

void ActivePlayer::InitializeVisibility(int32_t size_m, int32_t size_n) {
    visibility.resize(size_m);
    for (int32_t i = 0; i < size_m; i++) {
        visibility[i].resize(size_n);
        std::fill(visibility[i].begin(), visibility[i].end(), false);
    }
}
