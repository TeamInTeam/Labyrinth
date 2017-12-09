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
    std::pair<int32_t, int32_t> sizes = game_map_.GetSize();
    int32_t m = sizes.first, n = sizes.second;
    for (size_t i = 0; i < active_players_.size(); i++) {
        active_players_[i].InitializeVisibility(m, n);
    }
    std::srand (std::time(0));
    for (size_t i = 0; i < active_players_.size(); i++) {
        while (true) {
            bool flag = true;
            std::pair<int32_t, int32_t> sizes = game_map_.GetSize();
            int32_t m = 1 + rand() % (sizes.first - 1), n = 1 + rand() % (sizes.second - 1);
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
    result.second = active_players_[index].player_id;
    for (size_t i = 0; i < active_players_.size(); i++) {
        ShowTurn show_turn;
        show_turn.type_of_turn = TurnTypes::START;
        std::pair<int32_t, int32_t> sizes = game_map_.GetSize();
        show_turn.size_m = sizes.first;
        show_turn.size_n = sizes.second;
        show_turn.is_my_turn = (static_cast<int32_t>(i) == index);
        show_turn.m = active_players_[i].m;
        show_turn.n = active_players_[i].n;
        show_turn.health = active_players_[i].health;
        result.first.push_back(std::make_pair(active_players_[i].player_id, show_turn));
    }
    return result;
}

void Game::TerminatePlayerByID(int32_t player_id) {
    bool flag = false;
    size_t termination_index;
    for (size_t i = 0; i < active_players_.size(); i++) {
        if (active_players_[i].player_id == player_id) {
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
    switch (turn.type_of_turn) {
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
    case TurnTypes::SHOOT: {
        return ShootHandler(turn);
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
        show_turn.type_of_error = ErrorTypes::UNKNOWN_COMMAND;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }
    }
}

std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::WatchHandler(Turn turn) {
    ShowTurn show_turn;
    show_turn.type_of_turn = TurnTypes::WATCH;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;

    if (!CheckDistance(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DISTANCE;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }
    if (!CheckDirection(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DIRECTION;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }

    active_players_[index].visibility[turn.m][turn.n] = true;
    if (index == static_cast<int32_t>((index + 1) % active_players_.size())) {
        show_turn.is_my_turn = true;
    }

    auto items = game_map_.WatchItems(turn.m, turn.n);
    Encode(show_turn, turn.m, turn.n, std::get<0>(items), false);
    
    auto result = std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].player_id);
    index = (index + 1) % active_players_.size();
    return result;
}

std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::GoHandler(Turn turn) {
    ShowTurn show_turn;
    show_turn.type_of_turn = TurnTypes::GO;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;

    if (!CheckDistance(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DISTANCE;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }
    if (!CheckDirection(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DIRECTION;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }

    active_players_[index].visibility[turn.m][turn.n] = true;
    if (index == static_cast<int32_t>((index + 1) % active_players_.size())) {
        show_turn.is_my_turn = true;
    }
    
    switch (game_map_.GetType(turn.m, turn.n)) {
    case FieldTypes::ROAD: {
        auto sizes = game_map_.GetSize();
        if ((turn.m == sizes.first) || (turn.n == sizes.second)) {
            
        } else {
            auto items = game_map_.GetItems(turn.m, turn.n);
            ChangeActivePlayer(turn.m, turn.n, std::get<0>(items));
            Encode(show_turn, turn.m, turn.n, std::get<0>(items), true);
        }
        break;
    }
    case FieldTypes::WALL: {
        Encode(show_turn, turn.m, turn.n, false, false);
        break;
    }
    case FieldTypes::RIVER: {
        auto river_coordinates = game_map_.GetRiverCoordinates(turn.m, turn.n);
        auto items = game_map_.GetItems(river_coordinates[river_coordinates.size() - 1].first, river_coordinates[river_coordinates.size() - 1].second);
        ChangeActivePlayer(river_coordinates[river_coordinates.size() - 1].first, river_coordinates[river_coordinates.size() - 1].second, std::get<0>(items));
        Encode(show_turn, river_coordinates[river_coordinates.size() - 1].first, river_coordinates[river_coordinates.size() - 1].second, std::get<0>(items), true);
        show_turn.river_coordinates = river_coordinates;
        break;
    }
    case FieldTypes::METRO: {
        auto metro_coordinates = game_map_.GetTeleportCoordinates(turn.m, turn.n);
        auto items1 = game_map_.GetItems(turn.m, turn.n);
        auto items2 = game_map_.GetItems(metro_coordinates.first, metro_coordinates.second);
        ChangeActivePlayer(metro_coordinates.first, metro_coordinates.second, std::get<0>(items1) || std::get<0>(items2));
        Encode(show_turn, turn.m, turn.n, std::get<0>(items1) || std::get<0>(items2), true);
        show_turn.metro_m = metro_coordinates.first;
        show_turn.metro_n = metro_coordinates.second;
        break;
    }
    case FieldTypes::ARMOURY: {
        auto items = game_map_.GetItems(turn.m, turn.n);
        auto ammunition = game_map_.GetWeapon(turn.m, turn.n);
        ChangeActivePlayer(turn.m, turn.n, std::get<0>(items));
        active_players_[index].my_bombs += ammunition.first;
        active_players_[index].my_ammo += ammunition.second;
        Encode(show_turn, turn.m, turn.n, std::get<0>(items), true);
        show_turn.get_bombs = ammunition.first;
        show_turn.get_ammo = ammunition.second;
        break;
    }
    case FieldTypes::HOSPITAL: {
        auto items = game_map_.GetItems(turn.m, turn.n);
        auto medicine = game_map_.GetMedicine(turn.m, turn.n);
        ChangeActivePlayer(turn.m, turn.n, std::get<0>(items));
        active_players_[index].my_aids += medicine;
        Encode(show_turn, turn.m, turn.n, std::get<0>(items), true);
        show_turn.get_aids = medicine;
        break;
    }
    case FieldTypes::MINE: {
        active_players_[index].health = 0;
        if (active_players_[index].has_treasure) {
            active_players_[index].has_treasure = false;
            game_map_.PushItems(turn.m, turn.n, std::make_tuple(true, 0, 0, 0));
        }
        ChangeActivePlayer(turn.m, turn.n, false);
        Encode(show_turn, turn.m, turn.n, false, true);
        game_map_.ChangeType(turn.m, turn.n, FieldTypes::ROAD);
        break;
    }
    case FieldTypes::CONCRETE: {
        Encode(show_turn, turn.m, turn.n, false, false);
        break;
    }
    default: {
        show_turn.type_of_error = ErrorTypes::UNKNOWN_COMMAND;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }
    }

    auto result = std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].player_id);
    index = (index + 1) % active_players_.size();
    return result;
}

bool Game::IsHerePlayer(int32_t m, int32_t n) {
    for (size_t i = 0; i < active_players_.size(); i++)
        if ((active_players_[i].m == m) && (active_players_[i].n == n) && (static_cast<int32_t>(i) != index))
            return true;
    return false;
}

bool Game::CheckDistance(int32_t m, int32_t n) {
    return (active_players_[index].m - m) * (active_players_[index].m - m) + (active_players_[index].n - n) * (active_players_[index].n - n) == 1;
}

bool Game::CheckDirection(int32_t m, int32_t n) {
    return (active_players_[index].m == m) || (active_players_[index].n == n);
}

void Game::ChangeActivePlayer(int32_t m, int32_t n, bool get_treasure) {
    active_players_[index].m = m;
    active_players_[index].n = n;
    active_players_[index].has_treasure = (active_players_[index].has_treasure || get_treasure) && (active_players_[index].health != 0); 
}

void Game::Encode(ShowTurn &show_turn, int32_t m, int32_t n, bool get_treasure, bool was_moved) {
    show_turn.m = m;
    show_turn.n = n;
    show_turn.type_of_field = game_map_.GetType(m, n);
    show_turn.is_here_player = IsHerePlayer(m, n);
    show_turn.health = active_players_[index].health;
    show_turn.get_treasure = get_treasure;
    show_turn.was_moved = was_moved;
}
