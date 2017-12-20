/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef GAME_H_
#define GAME_H_

#include <cinttypes>
#include <vector>
#include <utility>
#include <tuple>
#include "activeplayer/activeplayer.h"
#include "gamemap/gamemap.h"
#include "proto/proto.h"

class Game {
 public:
    Game(GameMap game_map, std::vector<ActivePlayer> active_players);
    Game() = default;
    std::pair<std::vector<std::pair<int32_t, ShowTurn>>, int32_t> StartGame();
    void TerminatePlayerByID(int32_t player_id);
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> TurnHandler(Turn turn);

 private:
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> WatchHandler(Turn turn);
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> GoHandler(Turn turn);
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> KnifeHandler(Turn turn);
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> ShootHandler(Turn turn);
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> BombHandler(Turn turn);
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> ConcreteHandler(Turn turn);
    std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> AidHandler(Turn turn);

    void ChangeActivePlayer(int32_t m, int32_t n, bool get_treasure);
    
    void EncodeShowTurn(ShowTurn &show_turn, TurnTypes turn, int32_t previous_m, int32_t previous_n, int32_t current_m, int32_t current_n, bool treasure);

    bool Check(ShowTurn &show_turn, int32_t m, int32_t n, TurnTypes type_of_turn); 

    PlayerTypes IsHerePlayer(int32_t m, int32_t n);
    
    GameMap game_map_;
    
    std::vector<ActivePlayer> active_players_;
    int32_t index;
};

#endif  // GAME_H_
