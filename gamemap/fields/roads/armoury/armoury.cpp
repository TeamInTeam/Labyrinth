/* Copyright (C) 2017 Mikhail Masyagin */

#include <cinttypes>
#include "armoury.h"

Armoury::Armoury(int32_t my_bombs, int32_t my_ammo) :
    Roads(FieldTypes::ARMOURY),
    my_bombs(my_bombs),
    my_ammo(my_ammo) {}
