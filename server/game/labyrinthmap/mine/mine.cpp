/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./mine.h"

Mine::Mine(int32_t damage, int32_t visibility) :
    Field("Mine", true), damage_(damage), visibility_(visibility) {}

std::string Mine::GetType() const {
    return type_;
}

bool Mine::CanGo() const {
    return can_go_;
}

int32_t Mine::GetDamage() const {
    return damage_;
}

int32_t Mine::GetVisibility() const {
    return visibility_;
}
