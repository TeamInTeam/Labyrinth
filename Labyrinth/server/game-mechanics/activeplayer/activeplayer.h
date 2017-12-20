/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef ACTIVEPLAYER_H_
#define ACTIVEPLAYER_H_

#include <cinttypes>
#include <deque>
#include <string>

struct ActivePlayer {
    ActivePlayer(int32_t id, std::string name);
    ActivePlayer() = default;

    void InitializeVisibility(int32_t size_m, int32_t size_n);

    int32_t id;
    std::string name;

    int32_t m, n;
    
    int32_t health;
    
    bool treasure;
    int32_t bombs;
    int32_t ammo;
    int32_t concrete;
    int32_t aids;

    std::deque<std::deque<bool> > visibility;
};

#endif  // ACTIVEPLAYER_H_
