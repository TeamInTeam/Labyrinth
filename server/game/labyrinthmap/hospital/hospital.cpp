/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./hospital.h"

Hospital::Hospital(int32_t number_of_aids) :
    Field("Hospital", true) {
    number_of_aids_ = number_of_aids;
}

std::string Hospital::GetType() {
    return type_;
}

bool Hospital::CanGo() {
    return can_go_;
}

int32_t Hospital::NumberOfAids() {
    return number_of_aids_;
}

void Hospital::DecrementNumberOfAids() {
    if (number_of_aids_ > 0) {
        number_of_aids_--;
    }
}
