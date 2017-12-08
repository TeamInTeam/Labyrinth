/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef ARMOURY_H_
#define ARMOURY_H_

#include <cinttypes>
#include "../roads.h"

struct Armoury : public Roads {
    Armoury(int32_t my_bombs, int32_t my_ammo);

    int32_t my_bombs;
    int32_t my_ammo;
};

#endif  // ARMOURY_H_
