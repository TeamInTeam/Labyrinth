/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./wall.h"

Wall::Wall(bool is_destroyable) :
    Field("Wall", false) {
    is_destroyable_ = is_destroyable;
}

std::string Wall::GetType() {
    return type_;
}

bool Wall::IsDestroyable() {
    return is_destroyable_;
}

bool Wall::CanGo() {
    return can_go_;
}
