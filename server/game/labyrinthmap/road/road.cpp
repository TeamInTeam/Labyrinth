/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./road.h"

Road::Road() :
    Field("Road", true) {}

std::string Road::GetType() const {
    return type_;
}

bool Road::CanGo() const {
    return can_go_;
}
