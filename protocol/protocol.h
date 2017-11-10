/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef PROTOCOL_PROTOCOL_H_
#define PROTOCOL_PROTOCOL_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include <utility>

/*
  Player sends struct Turn to Server.
 */
struct Turn {
    // Player ID.
    uint64_t player_id;

    // Type of Turn.
    std::string type_of_turn;

    // coordinates
};

/*
  Server sends struct TurnGo to Current Player, 
  if his Type of Turn is Go.
 */
struct TurnGo {
    // Player ID.
    uint64_t player_id;

    // Is True if there is Error.
    bool error;

    // Is true if Player Can Go.
    bool can_go;

    // Coordinates.
    int32_t x, y;

    // Is True if there is Road.
    bool road;

    // Is True if there is Wall.
    bool wall;

    // Is True if there is Mine.
    bool mine;
    // Полученное ранение.

    // Is True if there is Hospital.
    bool hospital;
    // Полученная аптечка.

    // Is True if there is Armoury.
    bool armoury;
    // Полученное оружие.

    // Is True if there is Exit.
    bool exit;

    // Is True if there is River.
    bool river;
    std::vector<std::pair<int32_t, int32_t> > river_coordinates;

    // Is True if there is Enemy.
    bool is_here_enemy;
};

/*
  Server sends struct TurnGo to Waiting Players, 
  if Current Player's Type of Turn is Go.
 */
/*
struct TurnGoShow {

};
*/

#endif  // PROTOCOL_PROTOCOL_H_
