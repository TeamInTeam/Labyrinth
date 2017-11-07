/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_MAP_ROAD_ROAD_H_
#define SERVER_MAP_ROAD_ROAD_H_

#include <inttypes.h>
#include <string>
#include "../field/field.h"

/*
  Road Class.
 */
class Road : public Field {
 public:
    Road(bool has_treasure);

    // Returns Type of Field.
    std::string GetType();

    // Returns true if there is Treasure
    // in this field.
    bool HasTreasure();

    // Returns true if player can go
    // to this field.
    bool CanGo();

 private:
    bool has_treasure_;
};

#endif  // SERVER_MAP_ROAD_ROAD_H_
