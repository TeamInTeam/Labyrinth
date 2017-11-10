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
    Road();

    // Returns Type of Field.
    std::string GetType() const;

    // Returns true if player can go
    // to this field.
    bool CanGo() const;
};

#endif  // SERVER_MAP_ROAD_ROAD_H_
