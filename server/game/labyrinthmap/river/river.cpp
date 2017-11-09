/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include <vector>
#include <utility>
#include "./river.h"

River::River(int32_t x, int32_t y) :
    Field("River", true) {
    index_ = coordinates_.size();
    coordinates_.push_back(std::make_pair(x, y));
}

std::string River::GetType() {
    return type_;
}

bool River::CanGo() {
    return can_go_;
}

int32_t River::GetIndex() {
    return index_;
}
