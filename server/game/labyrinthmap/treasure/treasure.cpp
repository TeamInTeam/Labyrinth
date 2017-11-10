/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./treasure.h"

Treasure::Treasure() :
    Field("Treasure", true) {}

std::string Treasure::GetType() const {
    return type_;
}

bool Treasure::CanGo() const {
    return can_go_;
}
