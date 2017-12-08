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
                    if ((active_players_[j].current_m == m) &&
                        (active_players_[j].current_n == n)) {
                        flag = false;
                        break;
                    }
            } else {
                flag = false;
            }
            if (flag) {
                active_players_[i].current_m = m;
                active_players_[i].current_n = n;
                active_players_[i].previous_m = m;
                active_players_[i].previous_n = n;
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
                if ((active_players_[i].current_m == m) &&
                    (active_players_[i].current_n == n)) {
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
        show_turn.m = active_players_[i].current_m;
        show_turn.n = active_players_[i].current_n;
        show_turn.health = 5;
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

// Обработчик команды "WATCH".
std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::WatchHandler(Turn turn) {
    // Создание возвращаемых полей.
    ShowTurn show_turn;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;

    // Проверка на ошибки.
    if (!CheckDistance(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DISTANCE;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }
    if (!CheckDirection(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DIRECTION;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }

    // Обработка.
    active_players_[index].visibility[turn.m][turn.n] = true;
    if (index == static_cast<int32_t>((index + 1) % active_players_.size())) {
        show_turn.is_my_turn = true;
    }

    auto items = game_map_.WatchItems(turn.m, turn.n);

    // Отображение.
    show_turn.m = turn.m;
    show_turn.n = turn.n;
    show_turn.type_of_field = game_map_.GetType(turn.m, turn.n);
    show_turn.is_here_player = IsHerePlayer(turn.m, turn.n);
    show_turn.get_treasure = std::get<0>(items);

    // Отрисовка для других игроков.
    auto result = std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].player_id);
    index = (index + 1) % active_players_.size();
    return result;
}

// Обработчик команды "GO".
std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::GoHandler(Turn turn) {
    // Создание возвращаемых полей.
    ShowTurn show_turn;
    show_turn.type_of_turn = TurnTypes::GO;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;

    // Проверка на ошибки.
    if (!CheckDistance(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DISTANCE;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }
    if (!CheckDirection(turn.m, turn.n)) {
        show_turn.type_of_error = ErrorTypes::INVALID_DIRECTION;
        return std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }

    std::cout << "heh" << std::endl;

    // Обработка.
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
            // Обработка.
            active_players_[index].previous_m = active_players_[index].current_m;
            active_players_[index].previous_n = active_players_[index].current_n;
            active_players_[index].current_m = turn.m;
            active_players_[index].current_n = turn.n;
            active_players_[index].has_treasure = active_players_[index].has_treasure || std::get<0>(items); 
            
            // Отображение.
            show_turn.m = turn.m;
            show_turn.n = turn.n;
            show_turn.type_of_field = FieldTypes::ROAD;
            show_turn.is_here_player = IsHerePlayer(turn.n, turn.m);
            show_turn.health = active_players_[index].health;
            show_turn.get_treasure = std::get<0>(items);
            show_turn.was_moved = true;
        }
        break;
    }
    case FieldTypes::WALL: {
        // Обработка (нет).
        // Отображение.
        show_turn.m = turn.m;
        show_turn.n = turn.n;
        show_turn.type_of_field = FieldTypes::WALL;
        show_turn.health = active_players_[index].health;
        break;
    }
    case FieldTypes::RIVER: {
        auto river_coordinates = game_map_.GetRiverCoordinates(turn.m, turn.n);
        auto items = game_map_.GetItems(river_coordinates[river_coordinates.size() - 1].first, river_coordinates[river_coordinates.size() - 1].second);
        // Обработка.
        active_players_[index].previous_m = active_players_[index].current_m;
        active_players_[index].previous_n = active_players_[index].current_n;
        active_players_[index].current_m = river_coordinates[river_coordinates.size() - 1].first;
        active_players_[index].current_n = river_coordinates[river_coordinates.size() - 1].second;
        active_players_[index].has_treasure = active_players_[index].has_treasure || std::get<0>(items); 
        
        // Отображение.
        show_turn.m = river_coordinates[river_coordinates.size() - 1].first;
        show_turn.n = river_coordinates[river_coordinates.size() - 1].second;
        show_turn.river_coordinates = river_coordinates;
        show_turn.type_of_field = FieldTypes::RIVER;
        show_turn.is_here_player = IsHerePlayer(turn.n, turn.m);
        show_turn.health = active_players_[index].health;
        show_turn.get_treasure = std::get<0>(items);
        show_turn.was_moved = true;
        break;
    }
    case FieldTypes::METRO: {
        auto metro_coordinates = game_map_.GetTeleportCoordinates(turn.m, turn.n);
        auto items1 = game_map_.GetItems(turn.m, turn.n);
        auto items2 = game_map_.GetItems(metro_coordinates.first, metro_coordinates.second);
        // Обработка.
        active_players_[index].previous_m = active_players_[index].current_m;
        active_players_[index].previous_n = active_players_[index].current_n;
        active_players_[index].current_m = metro_coordinates.first;
        active_players_[index].current_n = metro_coordinates.second;
        active_players_[index].has_treasure = active_players_[index].has_treasure || std::get<0>(items1) || std::get<0>(items2); 
        
        // Отображение.
        show_turn.m = metro_coordinates.first;
        show_turn.n = metro_coordinates.second;
        show_turn.metro_m = metro_coordinates.first;
        show_turn.metro_n = metro_coordinates.second;
        show_turn.type_of_field = FieldTypes::METRO;
        show_turn.is_here_player = IsHerePlayer(metro_coordinates.first, metro_coordinates.second);
        show_turn.health = active_players_[index].health;
        show_turn.get_treasure = std::get<0>(items1) || std::get<0>(items2);
        show_turn.was_moved = true;
        break;
    }
    case FieldTypes::ARMOURY: {
        auto items = game_map_.GetItems(turn.m, turn.n);
        auto ammunition = game_map_.GetWeapon(turn.m, turn.n);
        // Обработка.
        active_players_[index].previous_m = active_players_[index].current_m;
        active_players_[index].previous_n = active_players_[index].current_n;
        active_players_[index].current_m = turn.m;
        active_players_[index].current_n = turn.n;
        active_players_[index].has_treasure = active_players_[index].has_treasure || std::get<0>(items);
        active_players_[index].my_bombs += ammunition.first;
        active_players_[index].my_ammo += ammunition.second;
        
        // Отображение.
        show_turn.m = turn.m;
        show_turn.n = turn.n;
        show_turn.type_of_field = FieldTypes::ARMOURY;
        show_turn.is_here_player = IsHerePlayer(turn.m, turn.n);
        show_turn.health = active_players_[index].health;
        show_turn.get_treasure = std::get<0>(items);
        show_turn.get_bombs = ammunition.first;
        show_turn.get_ammo = ammunition.second;
        show_turn.was_moved = true;
        break;
    }
    case FieldTypes::HOSPITAL: {
        auto items = game_map_.GetItems(turn.m, turn.n);
        auto medicine = game_map_.GetMedicine(turn.m, turn.n);
        // Обработка.
        active_players_[index].previous_m = active_players_[index].current_m;
        active_players_[index].previous_n = active_players_[index].current_n;
        active_players_[index].current_m = turn.m;
        active_players_[index].current_n = turn.n;
        active_players_[index].has_treasure = active_players_[index].has_treasure || std::get<0>(items);
        active_players_[index].my_aids += medicine;
        
        // Отображение.
        show_turn.m = turn.m;
        show_turn.n = turn.n;
        show_turn.type_of_field = FieldTypes::ARMOURY;
        show_turn.is_here_player = IsHerePlayer(turn.m, turn.n);
        show_turn.health = active_players_[index].health;
        show_turn.get_treasure = std::get<0>(items);
        show_turn.get_aids = medicine;
        show_turn.was_moved = true;
        break;
    }
    case FieldTypes::MINE: {
        // Обработка.
        
        // Отображение.
        break;
    }
    case FieldTypes::CONCRETE: {
        // Обработка (нет).
        // Отображение.
        show_turn.m = turn.m;
        show_turn.n = turn.n;
        show_turn.type_of_field = FieldTypes::CONCRETE;
        break;
    }
    default: {
        show_turn.type_of_error = ErrorTypes::UNKNOWN_COMMAND;
        std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[index].player_id);
    }
    }

    auto result = std::make_tuple(std::make_pair(active_players_[index].player_id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].player_id);
    index = (index + 1) % active_players_.size();
    return result;
}

bool Game::IsHerePlayer(int32_t m, int32_t n) {
    for (size_t i = 0; i < active_players_.size(); i++)
        if ((active_players_[i].current_m == m) && (active_players_[i].current_n == n) && (static_cast<int32_t>(i) != index))
            return true;
    return false;
}

bool Game::CheckDistance(int32_t m, int32_t n) {
    return (active_players_[index].current_m - m) * (active_players_[index].current_m - m) + (active_players_[index].current_n - n) * (active_players_[index].current_n - n) == 1;
}

bool Game::CheckDirection(int32_t m, int32_t n) {
    return (active_players_[index].current_m == m) || (active_players_[index].current_n == n);
}
