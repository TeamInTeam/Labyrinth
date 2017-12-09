/* Copyright (C) 2017 Mikhail Masyagin */

#include "metro.h"

Metro::Metro(size_t index) :
    Roads(FieldTypes::METRO),
    index(index),
    teleport_m(0),
    teleport_n(0) {}

Metro::Metro() : Roads(FieldTypes::METRO) {}
