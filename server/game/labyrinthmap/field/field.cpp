/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./field.h"

Field::Field(std::string type, bool can_go) : type_(type), can_go_(can_go) {}

std::string Field::GetType() const {
    return type_;
}

bool Field::CanGo() const {
    return can_go_;
}
