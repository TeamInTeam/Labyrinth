/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_LABYRINTHMAP_RIVER_RIVER_H_
#define SERVER_GAME_LABYRINTHMAP_RIVER_RIVER_H_

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

    // Returns Array's Index.
    int32_t GetIndex();

 private:
    // Index.
    int32_t index_;

    // River Coordinates.
    static std::vector<std::pair<int32_t, int32_t> > coordinates_;
};

#endif  // SERVER_GAME_LABYRINTHMAP_RIVER_RIVER_H_
