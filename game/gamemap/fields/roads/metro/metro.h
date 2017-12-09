/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef METRO_H_
#define METRO_H_

#include <cstring>
#include "../roads.h"

struct Metro : public Roads {
    Metro(size_t index);
    Metro();
    
    size_t index;
    size_t teleport_m, teleport_n;
};
#endif  // METRO_H_
