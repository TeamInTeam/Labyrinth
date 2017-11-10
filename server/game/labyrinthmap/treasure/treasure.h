/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_MAP_TREASURE_TREASURE_H_
#define SERVER_MAP_TREASURE_TREASURE_H_

#include <inttypes.h>
#include <string>
#include "../field/field.h"

/*
  Treasure Class.
 */
class Treasure : public Field {
 public:
    Treasure();

    // Returns Type of Field.
    std::string GetType() const;

    // Returns true if player can go
    // to this field.
    bool CanGo() const;
};

#endif  // SERVER_MAP_TREASURE_TREASURE_H_
