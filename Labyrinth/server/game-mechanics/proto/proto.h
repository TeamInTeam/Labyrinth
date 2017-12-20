/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef PROTO_H_
#define PROTO_H_

#include <cinttypes>
#include <string>
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
    TurnTypes turn;
    // Координаты.
    int32_t m, n;
};

enum class ErrorTypes {
    NO_ERR_IN_MSG = 0,
    NOT_MY_TURN = 1,
    UNKNOWN_COMMAND = 2,
    INVALID_DISTANCE = 3,
    INVALID_DIRECTION = 4,
    NO_AMMO = 5,
    NO_BOMB = 6,
    NO_CONCRETE = 7,
    NO_AID = 8,
    I_AM_DEAD = 9
};

enum class PlayerTypes {
    NO_PLAYER = 0,
    DEAD_PLAYER = 1,
    ALIVE_PLAYER = 2
};

struct ShowTurn {
    ShowTurn();
    
    // Ошибка.
    ErrorTypes error;
    // Тип хода.
    TurnTypes turn;

    // пары <никнейм, id>
    std::vector<std::pair<int32_t, std::string>> other_players;
    // Мой id.
    int32_t my_id;
    
    // Размеры карты.
    int32_t size_m, size_n;
    
    // Здоровье.
    int32_t health;
    // Сокровище.
    bool treasure;
    // Бомбы.
    int32_t bombs;
    // Патроны.
    int32_t ammo;
    // Бетон.
    int32_t concrete;
    // Аптечки.
    int32_t aids;
    
    // Координаты.
    int32_t m, n;
    // Для метро.
    int32_t metro_m, metro_n;
    // Для реки.
    std::vector<std::pair<int32_t, int32_t>> river_coordinates;
    // Тип клетки.
    FieldTypes field;

    // Другие игроки.
    PlayerTypes previous_player;
    PlayerTypes current_player;
    
    // Мой ли ход.
    bool my_turn;
    // Конец игры.
    bool game_end;
};

struct ShowOtherTurn {
    ShowOtherTurn();

    // Тип хода.
    TurnTypes turn;

    // Моё текущее здоровье.
    int32_t health;

    // id того, кто ходит.
    int32_t id;

    // Видны ли изначальные координаты.
    bool visible_from;
    // Изначальные координаты.
    int32_t previous_m, previous_n;
    // Тип изначальной клетки.
    FieldTypes previous_field;
    // Есть ли на ней клад.
    bool previous_treasure;
    // Другой игрок.
    PlayerTypes previous_player;
    
    // Видны ли конечные координаты.
    bool visible_to;
    // Конечные координаты.
    int32_t current_m, current_n;
    // Для метро.
    int32_t metro_m, metro_n;
    // Для реки.
    std::vector<std::pair<int32_t, int32_t>> river_coordinates;
    // Тип конечной клетки.
    FieldTypes current_field;
    // Есть ли на ней клад.
    bool current_treasure;
    // Другой игрок.
    PlayerTypes current_player;
    
    // Нашёл ли он сокровище.
    bool treasure;

    // Мой ли ход.
    bool my_turn;
    // Конец игры.
    bool game_end;
};

#endif  // PROTO_H_
