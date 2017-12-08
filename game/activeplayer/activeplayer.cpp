/* Copyright (C) 2017 Mikhail Masyagin */

#include <cinttypes>
#include <string>
#include <algorithm>
#include "activeplayer.h"

ActivePlayer::ActivePlayer(int32_t player_id, std::string player_name) :
    player_id(player_id),
    player_name(player_name),
    health(5),
    has_treasure(false),
    my_bombs(0),
    my_ammo(0),
    my_aids(0) {}

void ActivePlayer::InitializeVisibility(int32_t m, int32_t n) {
    visibility.resize(m);
    for (int32_t i = 0; i < m; i++) {
        visibility[i].resize(n);
        std::fill(visibility[i].begin(), visibility[i].end(), false);
    }
}
