/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./activeplayer.h"

ActivePlayer::ActivePlayer(uint64_t game_id,
                           uint64_t player_id,
                           std::string nickname,
                           std::string race,
                           int32_t health,
                           int32_t protection,
                           int32_t damage,
                           int32_t perception,
                           int32_t intellect,
                           int32_t fortune,
                           // Aids and Weapons.
                           int32_t size_x,
                           int32_t size_y,
                           int32_t current_x,
                           int32_t current_y) {
    game_id_ = game_id;

    player_id_ = player_id;
    nickname_ = nickname;
    race_ = race;

    health_ = health;
    protection_ = protection;

    damage_ = damage;
    perception_ = perception;
    intellect_ = intellect;
    fortune_ = fortune;

    size_x_ = size_x;
    size_y_ = size_y;
    visibility_of_labyrinthmap_.reserve(size_x);
    for (int32_t i = 0; i < size_x; i++) {
        visibility_of_labyrinthmap_[i].reserve(size_y);
        for (int32_t j = 0; j < size_y; j++)
            visibility_of_labyrinthmap_[i][j] = false;
    }
    visibility_of_labyrinthmap_[current_x][current_y] = true;

    current_x_ = current_x;
    current_y_ = current_y;
}

ActivePlayer::ActivePlayer() {
    game_id_ = 0;

    player_id_ = 0;
    nickname_ = "";
    race_ = "";

    health_ = 0;
    protection_ = 0;

    damage_ = 0;
    perception_ = 0;
    intellect_ = 0;
    fortune_ = 0;

    current_x_ = 0;
    current_y_ = 0;
}

int32_t ActivePlayer::GetHealth() {
    return health_;
}

bool ActivePlayer::SetHealth(int32_t health) {
    // Check
    health_ = health;
    return true;
}

int32_t ActivePlayer::GetProtection() {
    return protection_;
}

bool ActivePlayer::SetProtection(int32_t protection) {
    // Check
    protection_ = protection;
    return true;
}

bool ActivePlayer::IsVisible(int32_t i, int32_t j) {
    return visibility_of_labyrinthmap_[i][j];
}

void ActivePlayer::MakeVisible(int32_t i, int32_t j) {
    visibility_of_labyrinthmap_[i][j] = true;
}

bool ActivePlayer::IsAlive() {
    return (health_ > 0);
}
