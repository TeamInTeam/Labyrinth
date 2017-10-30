/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "../base/base.h"
#include "./armor.h"

Armor::Armor(std::string name,
             std::string description,
             int32_t wear,
             int32_t default_protection,
             int32_t current_protection) : BaseEquipment(name,
                                                         description,
                                                         wear) {
    default_protection_ = default_protection;
    current_protection_ = current_protection;
}

Armor::Armor() : BaseEquipment() {
    default_protection_ = 0;
    current_protection_ = 0;
}

std::string Armor::GetName() {
    return name_;
}

std::string Armor::GetDescription() {
    return description_;
}

int32_t Armor::GetDefaultProtection() {
    return default_protection_;
}

int32_t Armor::GetCurrentProtection() {
    return current_protection_;
}

bool Armor::IncrementWear() {
    wear_ += wear_step_;
    if (wear_ >= max_wear_)
        return true;
    current_protection_ = default_protection_ - default_protection_ / max_wear_ * wear_;
    return false;
}

