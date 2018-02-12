/* Copyright (C) 2017 Mikhail Masyagin */

#include "proto.h"

Turn::Turn() :
    turn(TurnTypes::WATCH),
    m(-1),
    n(-1) {};

ShowTurn::ShowTurn() :
    error(ErrorTypes::NO_ERR_IN_MSG),
    turn(TurnTypes::WATCH),
    my_id(-1),
    size_m(0), size_n(0),
    health(0),
    treasure(false),
    bombs(0),
    ammo(0),
    concrete(0),
    aids(0),
    m(-1), n(-1),
    metro_m(-1), metro_n(-1),
    field(FieldTypes::ROAD),
    previous_player(PlayerTypes::NO_PLAYER),
    current_player(PlayerTypes::NO_PLAYER),
    my_turn(false),
    game_end(false) {}

ShowOtherTurn::ShowOtherTurn() :
    turn(TurnTypes::WATCH),
    health(0),
    id(-1),
    visible_from(false),
    previous_m(-1), previous_n(-1),
    previous_field(FieldTypes::ROAD),
    previous_treasure(false),
    previous_player(PlayerTypes::NO_PLAYER),
    visible_to(false),
    current_m(-1), current_n(-1),
    metro_m(-1), metro_n(-1),
    current_field(FieldTypes::ROAD),
    current_treasure(false),
    current_player(PlayerTypes::NO_PLAYER),
    treasure(false),
    my_turn(false),
    game_end(false) {}
    
    
