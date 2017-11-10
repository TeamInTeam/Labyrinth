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

LabyrinthMap::LabyrinthMap(const LabyrinthMap &obj) {
    this->size_x_ = obj.size_x_;
    this->size_y_ = obj.size_y_;
    this->game_map_.reserve(size_x_);
    for (int32_t i = 0; i < size_x_; i++)
        this->game_map_[i].reserve(size_y_);
    for (int32_t i = 0; i < size_x_; i++)
        for (int32_t j = 0; j < size_y_; j++) {
            if (obj.GetType(i, j) == "Armoury") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Armoury*> (obj.game_map_[i][j]));
                continue;
            }
            if (obj.GetType(i, j) == "Hospital") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Armoury*> (obj.game_map_[i][j]));
                continue;
            }
            // Metro.
            if (obj.GetType(i, j) == "Mine") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Mine*> (obj.game_map_[i][j]));
                continue;
            }
            if (obj.GetType(i, j) == "River") {
                this->game_map_[i][i] = new River();
                continue;
            }
            if (obj.GetType(i, j) == "Road") {
                this->game_map_[i][i] = new Road();
                continue;
            }
            if (obj.GetType(i, j) == "Treasure") {
                this->game_map_[i][i] = new Treasure();
                continue;
            }
            if (obj.GetType(i, j) == "Wall") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Wall*> (obj.game_map_[i][j]));
                continue;
            }
        }
}

LabyrinthMap& LabyrinthMap::operator= (const LabyrinthMap &obj) {
    if (this != &obj) {
        for (int32_t i = 0; i < this->size_x_; i++)
            for (int32_t j = 0; j < this->size_y_; j++)
                delete this->game_map_[i][j];
        this->size_x_ = obj.size_x_;
        this->size_y_ = obj.size_y_;
        this->game_map_.reserve(this->size_x_);
        for (int32_t i = 0; i < this->size_x_; i++)
            this->game_map_[i].reserve(this->size_y_);
        for (int32_t i = 0; i < this->size_x_; i++)
            for (int32_t j = 0; j < this->size_y_; j++) {
                            if (obj.GetType(i, j) == "Armoury") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Armoury*> (obj.game_map_[i][j]));
                continue;
            }
            if (obj.GetType(i, j) == "Hospital") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Armoury*> (obj.game_map_[i][j]));
                continue;
            }
            // Metro.
            if (obj.GetType(i, j) == "Mine") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Mine*> (obj.game_map_[i][j]));
                continue;
            }
            if (obj.GetType(i, j) == "River") {
                this->game_map_[i][i] = new River();
                continue;
            }
            if (obj.GetType(i, j) == "Road") {
                this->game_map_[i][i] = new Road();
                continue;
            }
            if (obj.GetType(i, j) == "Treasure") {
                this->game_map_[i][i] = new Treasure();
                continue;
            }
            if (obj.GetType(i, j) == "Wall") {
                this->game_map_[i][i] = new Wall(dynamic_cast<Wall*> (obj.game_map_[i][j]));
                continue;
            }
            }
    }
    return *this;
}

LabyrinthMap::~LabyrinthMap() {
    for (int32_t i = 0; i < size_x_; i++)
        for (int32_t j = 0; j < size_y_; j++)
            delete game_map_[i][j];
}

int32_t LabyrinthMap::GetSizeX() const {
    return size_x_;
}

int32_t LabyrinthMap::GetSizeY() const {
    return size_y_;
}

bool LabyrinthMap::CanGo(int32_t i, int32_t j) const {
    return (dynamic_cast<Field*> (game_map_[i][j]))->CanGo();
}

std::string LabyrinthMap::GetType(int32_t i, int32_t j) const {
    return (dynamic_cast<Field*> (game_map_[i][j]))->GetType();
}
