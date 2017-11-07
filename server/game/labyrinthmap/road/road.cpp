/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./road.h"

Road::Road(bool has_treasure) :
    Field("Road", true) {
    has_treasure_ = has_treasure;
}

std::string Road::GetType() {
    return type_;
}

bool Road::HasTreasure() {
    return has_treasure_;
}

bool Road::CanGo() {
    return can_go_;
}
