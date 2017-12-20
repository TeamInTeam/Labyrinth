/* Copyright (C) 2017 Mikhail Masyagin */

#include "roads.h"

Roads::Roads(FieldTypes type_of_field) :
    Base(type_of_field),
    has_treasure(false),
    bombs(0),
    ammo(0),
    concrete(0),
    aids(0) {}
