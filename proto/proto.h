/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef PROTO_H_
#define PROTO_H_

#include <cinttypes>
#include <vector>
#include "../gamemap/fields/base/base.h"

enum class TurnTypes {
    START = 0,
    WATCH = 1,
    GO = 2,
    KNIFE = 3,
    SHOOT = 4,
    BOMB = 5,
    CONCRETE = 6,
    AID = 7,
    END = 8
};

struct Turn {
    Turn();
    
    // Тип хода.
    TurnTypes type_of_turn;
    // Координаты.
    int32_t m, n;
};

enum class ErrorTypes {
    NO_ERROR = 0,
    NOT_MY_TURN = 1,
    UNKNOWN_COMMAND = 2,
    INVALID_DISTANCE = 3,
    INVALID_DIRECTION = 4,
    NO_AMMO = 5,
    NO_BOMB = 6,
    NO_CONCRETE = 7,
    NO_AID = 8
};

struct ShowTurn {
    ShowTurn();
    
    // Ошибка.
    ErrorTypes type_of_error;
    // Тип хода.
    TurnTypes type_of_turn;
    
    // Размеры карты.
    int32_t size_m, size_n;
    
    // Координаты.
    int32_t m, n;
    // Для метро.
    int32_t metro_m, metro_n;
    // Для реки.
    std::vector<std::pair<int32_t, int32_t>> river_coordinates;
    // Тип клетки.
    FieldTypes type_of_field;
    // Есть ли на новой клетке игрок.
    bool is_here_player;
    // Есть ли в этой клетке монстр.
    bool is_here_monster;

    // Здоровье.
    int32_t health;
    
    // Сокровище.
    bool get_treasure;
    // Бомбы.
    int32_t get_bombs;
    // Патроны.
    int32_t get_ammo;
    // Аптечки.
    int32_t get_aids;

    // Был ли игрок перемещён.
    bool was_moved;
    // Мой ли ход.
    bool is_my_turn;
    // Конец игры.
    bool game_end;
};

struct ShowOtherTurn {
    
};

#endif  // PROTO_H_
