/* Copyright (C) 2017 Mikhail Masyagin */

#include <cstring>
#include <cinttypes>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <cmath>
#include "game.h"
#include <iostream>

Game::Game(GameMap game_map, std::vector<ActivePlayer> active_players) :
    game_map_(game_map),
    active_players_(active_players),
    index(0) {
    srand (time(0));
    int32_t m, n;
    std::tie(m, n) = game_map_.GetSize();
    for (size_t i = 0; i < active_players_.size(); i++) {
        active_players_[i].InitializeVisibility(m, n);
    }
    std::srand (std::time(0));
    for (size_t i = 0; i < active_players_.size(); i++) {
        while (true) {
            bool flag = true;
            std::pair<int32_t, int32_t> sizes = game_map_.GetSize();
            int32_t m = 1 + rand() % (sizes.first - 2), n = 1 + rand() % (sizes.second - 2);
            if (game_map_.GetType(m, n) == FieldTypes::ROAD) {
                for (size_t j = 0; j < i; j++)
                    if ((active_players_[j].m == m) &&
                        (active_players_[j].n == n)) {
                        flag = false;
                        break;
                    }
            } else {
                flag = false;
            }
            if (flag) {
                active_players_[i].m = m;
                active_players_[i].n = n;
                break;
            }
        }
    }
    
    while (true) {
        bool flag = true;
        std::pair<int32_t, int32_t> sizes = game_map_.GetSize();
        int32_t m = 1 + rand() % (sizes.first - 1), n = 1 + rand() % (sizes.second - 1);
        if (game_map_.GetType(m, n) == FieldTypes::ROAD) {
            for (size_t i = 0; i < active_players_.size(); i++)
                if ((active_players_[i].m == m) &&
                    (active_players_[i].n == n)) {
                    flag = false;
                    break;
                }
        } else {
            flag = false;
        }
        if (flag) {
            game_map_.PushItems(m, n, std::make_tuple(true, 0, 0, 0));
            break;
        }
    }
}

std::pair<std::vector<std::pair<int32_t, ShowTurn>>, int32_t> Game::StartGame() {
    std::pair<std::vector<std::pair<int32_t, ShowTurn>>, int32_t> result;
    result.second = active_players_[index].id;
    for (index = 0; index < static_cast<int32_t>(active_players_.size()); index++) {
        ShowTurn show_turn;
        std::tie(show_turn.size_m, show_turn.size_n) = game_map_.GetSize();
        active_players_[index].visibility[active_players_[index].m][active_players_[index].n] = true;
        EncodeShowTurn(show_turn, TurnTypes::START, active_players_[index].m, active_players_[index].n, active_players_[index].m, active_players_[index].n, false);
        result.first.push_back(std::make_pair(active_players_[index].id, show_turn));
    }
    index = 0;
    result.first[index].second.my_turn = true;
    return result;
}

void Game::TerminatePlayerByID(int32_t id) {
bool flag = false;
size_t termination_index;
    for (size_t i = 0; i < active_players_.size(); i++) {
        if (active_players_[i].id == id) {
            termination_index = i;
            flag = true;
            break;
        }
    }

    if (!flag) return;
    
    // Перерисовка.

    active_players_.erase(active_players_.begin() + termination_index);
}

std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::TurnHandler(Turn turn) {

    switch (turn.turn) {
    case TurnTypes::WATCH: {
        return WatchHandler(turn);
    }
    case TurnTypes::GO: {
        return GoHandler(turn);
    }
        /*
    case TurnTypes::KNIFE: {
        return KnifeHandler(turn);
    }
    case TurnTypes::BOMB: {
        return BombHandler(turn);
    }
    case TurnTypes::CONCRETE: {
        return ConcreteHandler(turn);
    }
    case TurnTypes::AID: {
        return AidHandler(turn);
    }
        */
    default: {
        ShowTurn show_turn;
        std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;
        show_turn.error = ErrorTypes::UNKNOWN_COMMAND;
        show_turn.my_turn = true;
        return std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[index].id);
    }
    }
}

bool Game::Check(ShowTurn &show_turn, int32_t m, int32_t n, TurnTypes type_of_turn) {
    if ((type_of_turn != TurnTypes::AID) && (type_of_turn != TurnTypes::SHOOT) &&
        (!((active_players_[index].m - m) * (active_players_[index].m - m) +
           (active_players_[index].n - n) * (active_players_[index].n - n) == 1))) {
        show_turn.error = ErrorTypes::INVALID_DISTANCE;
        return false;
    }
    if ((type_of_turn != TurnTypes::AID) && (type_of_turn != TurnTypes::SHOOT) &&
        (!((active_players_[index].m == m) || (active_players_[index].n == n)))) {
        show_turn.error = ErrorTypes::INVALID_DIRECTION;
        return false;
    }
    if ((type_of_turn != TurnTypes::WATCH) && (type_of_turn != TurnTypes::GO) &&
        (active_players_[index].health <= 0)) {
        show_turn.error = ErrorTypes::I_AM_DEAD;
        return false;
    }
    if ((type_of_turn == TurnTypes::SHOOT) &&
        (active_players_[index].ammo <= 0)) {
        show_turn.error = ErrorTypes::NO_AMMO;
        return false;
    }
    if ((type_of_turn == TurnTypes::BOMB) &&
        (active_players_[index].bombs <= 0)) {
        show_turn.error = ErrorTypes::NO_BOMB;
        return false;
    }
    if ((type_of_turn == TurnTypes::CONCRETE) &&
        (active_players_[index].concrete <= 0)) {
        show_turn.error = ErrorTypes::NO_CONCRETE;
        return false;
    }
    if ((type_of_turn == TurnTypes::AID) &&
        (active_players_[index].aids <= 0)) {
        show_turn.error = ErrorTypes::NO_AID;
        return false;
    }
    if ((type_of_turn != TurnTypes::AID) && (type_of_turn != TurnTypes::SHOOT)) {
        active_players_[index].visibility[m][n] = true;
    }
    if (index == static_cast<int32_t>((index + 1) % active_players_.size())) {
        show_turn.my_turn = true;
    }
    return true;
}
