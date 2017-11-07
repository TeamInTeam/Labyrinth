/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_MAP_RIVER_RIVER_H_
#define SERVER_MAP_RIVER_RIVER_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include <utility>
#include "../field/field.h"

/*
  River class
*/
class River : public Field {
 public:
    River();

    // Returns Type of Field.
    std::string GetType();

    // Returns true if player can go
    // to this field.
    bool CanGo();
};

#endif  // SERVER_MAP_RIVER_RIVER_H_
