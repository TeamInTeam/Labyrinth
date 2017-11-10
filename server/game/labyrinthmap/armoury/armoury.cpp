/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./armoury.h"

Armoury::Armoury(int32_t number_of_weapons) :
    Field("Armoury", true), number_of_weapons_(number_of_weapons) {}

std::string Armoury::GetType() const {
    return type_;
}

bool Armoury::CanGo() const {
    return can_go_;
}

int32_t Armoury::NumberOfWeapons() const {
    return number_of_weapons_;
}

void Armoury::DecrementNumberOfWeapons() {
    if (number_of_weapons_ > 0) {
        number_of_weapons_--;
    }
}
