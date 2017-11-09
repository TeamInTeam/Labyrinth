/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./treasure.h"

Treasure::Treasure() :
    Field("Treasure", true) {}

std::string Treasure::GetType() {
    return type_;
}

bool Treasure::CanGo() {
    return can_go_;
}
