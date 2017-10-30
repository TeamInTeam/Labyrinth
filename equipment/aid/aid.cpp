/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "../base/base.h"
#include "./aid.h"

Aid::Aid(std::string name,
             std::string description,
             int32_t wear,
             int32_t default_heal,
             int32_t current_heal) : BaseEquipment(name,
                                                         description,
                                                         wear) {
    default_heal_ = default_heal;
    current_heal_ = current_heal;
}

Aid::Aid() : BaseEquipment() {
    default_heal_ = 0;
    current_heal_ = 0;
}

std::string Aid::GetName() {
    return name_;
}

std::string Aid::GetDescription() {
    return description_;
}

int32_t Aid::GetDefaultHeal() {
    return default_heal_;
}

int32_t Aid::GetCurrentHeal() {
    return current_heal_;
}

bool Aid::IncrementWear() {
    wear_ += wear_step_;
    if (wear_ >= max_wear_)
        return true;
    current_heal_ = default_heal_ - default_heal_ / max_wear_ * wear_;
    return false;
}

