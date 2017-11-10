/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./wall.h"

Wall::Wall(bool is_destroyable) :
    Field("Wall", false), is_destroyable_(is_destroyable) {}

std::string Wall::GetType() const {
    return type_;
}

bool Wall::IsDestroyable() const {
    return is_destroyable_;
}

bool Wall::CanGo() const {
    return can_go_;
}
