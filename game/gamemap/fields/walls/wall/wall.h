/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef WALL_H_
#define WALL_H_

#include "../../base/base.h"

struct Wall : public Base {
    explicit Wall(bool is_destroyable);
    
    bool is_destroyable;
};

#endif  // WALL_H_
