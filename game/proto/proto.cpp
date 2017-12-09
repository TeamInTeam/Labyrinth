/* Copyright (C) 2017 Mikhail Masyagin */

#include "proto.h"

Turn::Turn() :
    type_of_turn(TurnTypes::WATCH),
    m(-1),
    n(-1) {};

ShowTurn::ShowTurn() :
    type_of_error(ErrorTypes::NO_ERROR),
    type_of_turn(TurnTypes::WATCH),
    size_m(-1),
    size_n(-1),
    m(-1),
    n(-1),
    metro_m(-1),
    metro_n(-1),
    type_of_field(FieldTypes::ROAD),
    is_here_player(false),
    is_here_monster(false),
    health(0),
    get_treasure(false),
    get_bombs(0),
    get_ammo(0),
    get_aids(0),
    was_moved(false),
    is_my_turn(false),
    game_end(false) {}
