/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_GAME_H_
#define SERVER_GAME_GAME_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include "./activeplayer/activeplayer.h"
#include "./labyrinthmap/labyrinthmap.h"
#include "./labyrinthmap/field/field.h"
#include "./labyrinthmap/road/road.h"
#include "./labyrinthmap/wall/wall.h"
#include "./labyrinthmap/river/river.h"
#include "./labyrinthmap/metro/metro.h"
#include "./labyrinthmap/armoury/armoury.h"
#include "./labyrinthmap/hospital/hospital.h"
#include "./labyrinthmap/mine/mine.h"
#include "./labyrinthmap/treasure/treasure.h"
#include "../../protocol/protocol.h"

/*
  Game Class.
 */
class Game {
 public:
    // Constructor.
    Game(uint64_t game_id,
         std::vector<ActivePlayer> players,
         int size_x, int size_y, int **map_scheme);
    // Default Constructor to be able to create
    // arrays of ActivePlayers.
    Game();

    // Turn Handler.
    struct TurnGo TurnHandler(struct Turn turn);

 private:
    // Game Id.
    uint64_t game_id_;
    
    // Players.
    std::vector<ActivePlayer> players_;

    // Current Player Index.
    int current_player_index_;

    // Game Map sizes.
    int32_t size_x_, size_y_;
    // Game Map.
    LabyrinthMap game_map_;
};

#endif  // SERVER_GAME_GAME_H_
