/* Copyright (C) 2017 Mikhail Masyagin & Madina Atymkhanova */

#ifndef SERVER_GAME_LABYRINTHMAP_LABYRINTHMAP_H_
#define SERVER_GAME_LABYRINTHMAP_LABYRINTHMAP_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include "./field/field.h"
#include "./road/road.h"
#include "./wall/wall.h"
#include "./river/river.h"
#include "./metro/metro.h"
#include "./armoury/armoury.h"
#include "./hospital/hospital.h"
#include "./mine/mine.h"
#include "./treasure/treasure.h"

class LabyrinthMap {
 public:
    // Constructor.
    // Takes int32_t arr size_x * size_y.
    // 00 - Road;
    // 11 - Undestroyable Wall, 21 - Destroyable Wall;
    // 02 - River;
    // x3 - Metro;
    // 04 - Armoury;
    // 05 - Hospital;
    // x6 - Mine;
    // 07 - Treasure.
    LabyrinthMap(int32_t size_x, int32_t size_y, int32_t **arr);
    // Copy Constructor.
    LabyrinthMap(const LabyrinthMap &obj);
    // Affectation Operator.
    LabyrinthMap& operator= (const LabyrinthMap &obj);
    // Destructor.
    ~LabyrinthMap();

    // Returns true, if Player can go on this Field.
    bool CanGo(int32_t i, int32_t j) const;

    // Returns Field Type.
    std::string GetType(int32_t i, int32_t j) const;

 private:
    // Width and Length of Game Map.
    int size_x_, size_y_;
    // Game Map.
    std::vector<std::vector<Field*> > game_map_;
};

#endif  // SERVER_GAME_LABYRINTHMAP_LABYRINTHMAP_H_
