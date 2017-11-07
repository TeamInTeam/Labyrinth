/* Copyright (C) 2017 Mikhail Masyagin & Madina Atymkhanova */

#ifndef SERVER_MAP_LABYRINTHMAP_H_
#define SERVER_MAP_LABYRINTHMAP_H_

#include <inttypes.h>
#include "./field/field.h"

class LabyrinthMap {
 public:
    // Constructor.
    // Takes int32_t arr size_x * size_y.
    // 0 - Road;
    // 11 - Undestroyable Wall, 12 - Destroyable Wall;
    // 2 - River;
    // 3x - Metro;
    // 41 - Ammunition, 42 - Ambulatory;
    // 5 - Mine
    // 10 - Treasure.
    LabyrinthMap(int32_t size_x, int32_t size_y, int32_t arr[6][6]);
    // Deleted Constructors.
    LabyrinthMap(const LabyrinthMap &obj) = delete;
    LabyrinthMap& operator= (const LabyrinthMap &obj) = delete;
    // Destructor.
    //~LabyrinthMap();

    bool CanGo(int32_t i, int32_t j);

 private:
    // Width and Length of Game Map.
    int size_x_, size_y_;
    // Game Map.
    Field*** game_map_;
};


#endif  // SERVER_MAP_LABYRINTHMAP_H_
