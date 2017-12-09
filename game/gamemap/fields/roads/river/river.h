/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef RIVER_H_
#define RIVER_H_

#include <vector>
#include <cstring>
#include <utility>
#include "../roads.h"

struct River : public Roads {
    River(std::vector<std::pair<int32_t, int32_t>> coordinates);
    
    std::vector<std::pair<int32_t, int32_t>> coordinates;
};

#endif  // RIVER_H_
