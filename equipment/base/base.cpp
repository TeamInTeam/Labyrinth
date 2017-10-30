/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./base.h"

BaseEquipment::BaseEquipment(std::string name,
                             std::string description,
                             int32_t wear) {
    // Check
    name_ = name;
    description_ = description;
    wear_ = wear;
}

BaseEquipment::BaseEquipment() {
    name_ = "";
    description_ = "";
    wear_ = 0;
}

std::string BaseEquipment::GetName() {
    return name_;
}

std::string BaseEquipment::GetDescription() {
    return description_;
}

bool BaseEquipment::IncrementWear() {
    wear_ += wear_step_;
    if (wear_ >= max_wear_)
        return true;
    return false;
}
