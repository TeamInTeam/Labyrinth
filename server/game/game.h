/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_GAME_H_
#define SERVER_GAME_GAME_H_

#include <inttypes.h>
#include <string>
#include "./activeplayer/activeplayer.h"
#include "./labyrinthmap/field/field.h"
#include "./labyrinthmap/road/road.h"
#include "./labyrinthmap/wall/wall.h"
#include "./labyrinthmap/river/river.h"

/*
  Game Class.
 */
class Game {
 public:
    Game(uint64_t game_id,
         std::vector<ActivePlayer> players);
    // Default Constructor to be able to create
    // arrays of ActivePlayers.
    Game();
};

#endif  // SERVER_GAME_GAME_H_
