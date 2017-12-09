/* Copyright (C) 2017 Mikhail Masyagin */

#include <iostream>
#include <cinttypes>
#include <vector>
#include <utility>
#include "river.h"

River::River(std::vector<std::pair<int32_t, int32_t>> coordinates) :
    Roads(FieldTypes::RIVER),
    coordinates(coordinates) {}
