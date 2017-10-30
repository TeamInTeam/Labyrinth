/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "../base/base.h"
#include "./weapon.h"

Weapon::Weapon(std::string name,
               std::string description,
               int32_t wear,
               int32_t default_damage,
               int32_t current_damage,
               int32_t max_distance) : BaseEquipment(name,
                                                     description,
                                                     wear) {
    default_damage_ = default_damage;
    current_damage_ = current_damage;
    max_distance_ = max_distance;
}

Weapon::Weapon() : BaseEquipment() {
    default_damage_ = 0;
    current_damage_ = 0;
}

std::string Weapon::GetName() {
    return name_;
}

std::string Weapon::GetDescription() {
    return description_;
}

int32_t Weapon::GetDefaultDamage() {
    return default_damage_;
}

int32_t Weapon::GetCurrentDamage() {
    return current_damage_;
}

int32_t Weapon::GetMaxDistance() {
    return max_distance_;
}

bool Weapon::IncrementWear() {
    wear_ += wear_step_;
    if (wear_ >= max_wear_)
        return true;
    current_damage_ = default_damage_ - default_damage_ / max_wear_ * wear_;
    return false;
}
