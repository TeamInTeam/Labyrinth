/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef ROADS_H_
#define ROADS_H_

#include <cinttypes>
#include "../base/base.h"

struct Roads : public Base {
    Roads(FieldTypes type_of_field);

    bool has_treasure;
    int32_t bombs;
    int32_t ammo;
    int32_t concrete;
    int32_t aids;
};

#endif  // ROADS_H_
