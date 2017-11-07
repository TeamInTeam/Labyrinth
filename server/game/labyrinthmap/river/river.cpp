/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include <vector>
#include <utility>
#include "./river.h"

River::River() :
    Field("River", true) {
}

std::string River::GetType() {
    return type_;
}

bool River::CanGo() {
    return can_go_;
}
