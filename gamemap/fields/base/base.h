/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef BASE_H_
#define BASE_H_

enum class FieldTypes {
    ROAD = 0,
    WALL = 1,
    RIVER = 2,
    METRO = 3,
    ARMOURY = 4,
    HOSPITAL = 5,
    MINE = 6,
    CONCRETE = 7,
    FOG = 8
};

struct Base {
    Base(FieldTypes type_of_field);

    FieldTypes type_of_field;
};

#endif  // BASE_H_
