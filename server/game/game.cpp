/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
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
#include "./game.h"

Game::Game(uint64_t game_id,
           std::vector<ActivePlayer> players,
           int size_x, int size_y, int **map_scheme) {
    game_id_ = game_id;
    
    players_ = players;

    current_player_index_ = 0;
    
    size_x_ = size_x;
    size_y_ = size_y;
    game_map_ = LabyrinthMap(size_x, size_y, map_scheme);
}
