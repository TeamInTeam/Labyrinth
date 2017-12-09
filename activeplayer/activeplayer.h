/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef ACTIVEPLAYER_H_
#define ACTIVEPLAYER_H_

#include <cinttypes>
#include <deque>
#include <string>

struct ActivePlayer {
    ActivePlayer(int32_t player_id, std::string player_name);
    ActivePlayer() = default;

    void InitializeVisibility(int32_t m, int32_t n);

    int32_t player_id;
    std::string player_name;

    int32_t health;
    
    bool has_treasure;
    int32_t my_bombs;
    int32_t my_ammo;
    int32_t my_concretes;
    int32_t my_aids;
    
    int32_t m, n;

    std::deque<std::deque<bool> > visibility;
};

#endif  // ACTIVEPLAYER_H_
