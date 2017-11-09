/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./road.h"

Road::Road() :
    Field("Road", true) {}

std::string Road::GetType() {
    return type_;
}

bool Road::CanGo() {
    return can_go_;
}
