/* Copyright (C) 2017 Mikhail Masyagin & Madina Atymkhanova */

#include <iostream>
#include "./labyrinthmap.h"
#include "./field/field.h"
#include "./road/road.h"
#include "./wall/wall.h"
#include "./river/river.h"

LabyrinthMap::LabyrinthMap(int32_t size_x, int32_t size_y, int arr[6][6]) {
    size_x_ = size_x;
    size_y_ = size_y;

    game_map_ = new Field**[size_x];
    for (int32_t i = 0; i < size_x; i++)
        game_map_[i] = new Field*[size_y];

    for (int32_t i = 0; i < size_x; i++)
        for (int32_t j = 0; j < size_y; j++) {
            switch (arr[i][j]) {
            case 0: {
                game_map_[i][j] = new Road(false);
                break;
            }
            case 11: {
                game_map_[i][j] = new Wall(false);
                break;
            }
            case 12: {
                game_map_[i][j] = new Wall(true);
                break;
            }
            case 2: {
                game_map_[i][j] = new River();
                break;
            }
                /* Дополнить */
            case 10: {
                game_map_[i][j] = new Road(true);
                break;
            }
            }
        }
}

/*
LabyrinthMap::~LabyrinthMap() {
    for (int32_t i = 0; i < size_x_; i++) {
        for (int32_t j = 0; j < size_y_; j++) {
            //
        }
        delete[] game_map_[i];
    }
    delete[] game_map_;
}
*/

bool LabyrinthMap::CanGo(int32_t i, int32_t j) {
    std::cout << (dynamic_cast<Field*> (game_map_[i][j]))->CanGo() << std::endl;
    return (dynamic_cast<Field*> (game_map_[i][j]))->CanGo();
}
