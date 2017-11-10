/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include <vector>
#include <utility>
#include "./river.h"

River::River(int32_t x, int32_t y) :
    Field("River", true) {}

std::string River::GetType() const {
    return type_;
}

bool River::CanGo() const {
    return can_go_;
}

