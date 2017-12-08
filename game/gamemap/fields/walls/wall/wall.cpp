/* Copyright (C) 2017 Mikhail Masyagin */

#include "wall.h"

Wall::Wall(bool is_destroyable) :
    Base(FieldTypes::WALL),
    is_destroyable(is_destroyable) {}
