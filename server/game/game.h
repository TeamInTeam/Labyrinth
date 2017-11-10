/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_GAME_H_
#define SERVER_GAME_GAME_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include "./activeplayer/activeplayer.h"
#include "./labyrinthmap/labyrinthmap.h"
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
    
    // Game Map.
    LabyrinthMap game_map_;
};

#endif  // SERVER_GAME_GAME_H_
