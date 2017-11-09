/* Copyright (C) 2017 Mikhail Masyagin & Madina Atymkhanova */

#include <inttypes.h>
#include <string>
#include <vector>
#include "./labyrinthmap.h"
#include "./field/field.h"
#include "./road/road.h"
#include "./wall/wall.h"
#include "./river/river.h"
#include "./metro/metro.h"
#include "./armoury/armoury.h"
#include "./hospital/hospital.h"
#include "./mine/mine.h"
#include "./treasure/treasure.h"

LabyrinthMap::LabyrinthMap(int32_t size_x, int32_t size_y, int32_t **arr) {
    size_x_ = size_x;
    size_y_ = size_y;

    game_map_.reserve(size_x);
    for (int32_t i = 0; i < size_x; i++)
        game_map_[i].reserve(size_y);

    for (int32_t i = 0; i < size_x; i++)
        for (int32_t j = 0; j < size_y; j++) {
            switch (arr[i][j] % 10) {
            case 1: {
                if (arr[i][j] == 11)
                    game_map_[i][j] = new Wall(false);
                if (arr[i][j] == 21)
                    game_map_[i][j] = new Wall(true);
                break;
            }
            case 2: {
                game_map_[i][j] = new River();
                break;
            }
            case 3: {
                // Метро
                break;
            }
            case 4: {
                // Добавить кол-во в зависимости от номера
                game_map_[i][j] = new Armoury(100);
                break;
            }
            case 5: {
                // Добавить кол-во в зависимости от номера
                game_map_[i][j] = new Hospital(100);
                break;
            }
            case 6: {
                // Добавить свойства в зависимости от номера
                game_map_[i][j] = new Mine(100, 100);
                break;
            }
            case 7: {
                game_map_[i][j] = new Treasure();
                break;
            }
            default: {
                game_map_[i][j] = new Road();
                break;
            }
            }
        }
}

LabyrinthMap::~LabyrinthMap() {
    for (int32_t i = 0; i < size_x_; i++)
        for (int32_t j = 0; j < size_y_; j++)
            delete game_map_[i][j];
}

bool LabyrinthMap::CanGo(int32_t i, int32_t j) {
    return (dynamic_cast<Field*> (game_map_[i][j]))->CanGo();
}

std::string LabyrinthMap::GetType(int32_t i, int32_t j) {
    return (dynamic_cast<Field*> (game_map_[i][j]))->GetType();
}
