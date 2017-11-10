/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include "./activeplayer/activeplayer.h"
#include "./labyrinthmap/labyrinthmap.h"
#include "../../protocol/protocol.h"
#include "./game.h"

Game::Game(uint64_t game_id,
           std::vector<ActivePlayer> players,
           int size_x, int size_y, int **map_scheme) :
    game_id_(game_id),
    players_(players),
    current_player_index_(0),
    game_map_(LabyrinthMap(size_x, size_y, map_scheme)) {}

Game::Game() : game_id_(0), current_player_index_(0) {}
