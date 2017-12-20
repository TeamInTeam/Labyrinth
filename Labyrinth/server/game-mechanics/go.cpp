/* Copyright (C) 2017 Mikhail Masyagin */

#include <cstring>
#include <cinttypes>
#include <utility>
#include <tuple>
#include "game.h"
#include <iostream>

// GoHandler вовзращает сообщения сходившему игроку, ждущим игрокам, а также индекс вновь ходящего игрока. 
std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::GoHandler(Turn turn) {
    // Создание дефолтных исходящих сообщний.
    ShowTurn show_turn;
    show_turn.turn = TurnTypes::GO;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;

    // Проверка коректности входящего сообщения.
    if (!((active_players_[index].m - turn.m) * (active_players_[index].m - turn.m) +
           (active_players_[index].n - turn.n) * (active_players_[index].n - turn.n) == 1)) {
        show_turn.error = ErrorTypes::INVALID_DISTANCE;
        show_turn.my_turn = true;
        return std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[index].id);
    }
    if  (!((active_players_[index].m == turn.m) || (active_players_[index].n == turn.n))) {
        show_turn.error = ErrorTypes::INVALID_DIRECTION;
        show_turn.my_turn = true;
        return std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[index].id);
    }
    
    // Создание сообщений для других игроков.
    for (size_t i = 0; i < active_players_.size(); i++) {
        ShowOtherTurn show_other_turn;
        if (static_cast<int32_t>(i) != index) {
            show_other_turn.turn = turn.turn;
            if (active_players_[i].visibility[active_players_[index].m][active_players_[index].n]) {
                show_other_turn.visible_from = true;
                show_other_turn.previous_m = active_players_[index].m;
                show_other_turn.previous_n = active_players_[index].n;
                show_other_turn.previous_field = game_map_.GetType(active_players_[index].m, active_players_[index].n);
                show_other_turn.previous_player = IsHerePlayer(active_players_[index].m, active_players_[index].n);
                show_other_turn.previous_treasure = std::get<0>(game_map_.WatchItems(active_players_[index].m, active_players_[index].n));
            }
            if (active_players_[i].visibility[turn.m][turn.n]) {
                show_other_turn.visible_to = true;
                show_other_turn.current_m = turn.m;
                show_other_turn.current_n = turn.n;
                show_other_turn.current_field = game_map_.GetType(turn.m, turn.n);
            }
            show_other_turn.my_turn = (i == (index + 1) % active_players_.size());
        }
        show_other_turns.push_back(std::make_pair(active_players_[i].id, show_other_turn));
    }
	
    // Добавление очередной клетки в поле видимости игрока.
    active_players_[index].visibility[turn.m][turn.n] = true;

    // Создание сообщения для сходившего игрока.
    switch (game_map_.GetType(turn.m, turn.n)) {
    case FieldTypes::ROAD: {
        auto sizes = game_map_.GetSize();
        if ((turn.m == 0) || (turn.n == 0) || (turn.m == sizes.first - 1) || (turn.n == sizes.second - 1)) {
            if (active_players_[index].treasure) {
                // Завершение игры.
                // Создание сообщений для ждущих игроков.
                for (size_t i = 0; i < show_other_turns.size(); i++)
                    if (static_cast<int32_t>(i) != index) {
                        show_other_turns[i].second.game_end = true;
                    }
                // Создание сообщения для сходившего игрока.
                show_turn.game_end = true;
                // Отправка сообщений.
                show_other_turns.erase(show_other_turns.begin() + index);
                return std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, -1);
            } else {
                // Создание сообщения для сходившего игрока.
                EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, false);
            }
        } else {
            if (active_players_[index].health > 0) {
                // Создание сообщений для ждущих игроков.
                for (size_t i = 0; i < show_other_turns.size(); i++)
                    if (static_cast<int32_t>(i) != index) {
                        if (show_other_turns[i].second.visible_to) {
                            show_other_turns[i].second.current_player = PlayerTypes::ALIVE_PLAYER;
                        }
                    }
                // Создание сообщения для сходившего игрока.
                auto tr = std::get<0>(game_map_.GetItems(turn.m, turn.n));
                active_players_[index].treasure = active_players_[index].treasure || tr;
                EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, tr);
            } else {
                // Создание сообщений для ждущих игроков.
                for (size_t i = 0; i < show_other_turns.size(); i++)
                    if (static_cast<int32_t>(i) != index) {
                        if (show_other_turns[i].second.visible_to) {
                            show_other_turns[i].second.current_player = PlayerTypes::DEAD_PLAYER;
                            show_other_turns[i].second.current_treasure = std::get<0>(game_map_.WatchItems(turn.m, turn.n));
                        }
                    }
                // Создание сообщения для сходившего игрока.
                EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, std::get<0>(game_map_.WatchItems(turn.m, turn.n)));
            }
            active_players_[index].m = turn.m;
            active_players_[index].n = turn.n;
        }
        break;
    }
    case FieldTypes::WALL: {
        // Создание сообщения для сходившего игрока.
        EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, false);
        break;
    }
    case FieldTypes::RIVER: {
        auto river_coordinates = game_map_.GetRiverCoordinates(turn.m, turn.n);
        for (size_t i = 0; i < river_coordinates.size(); i++)
            active_players_[index].visibility[river_coordinates[i].first][river_coordinates[i].second] = true;
        if (active_players_[index].health > 0) {
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    for (size_t j = 0; j < river_coordinates.size(); j++)
                        if (active_players_[i].visibility[river_coordinates[j].first][river_coordinates[j].second]) {
                            show_other_turns[i].second.visible_to = true;
                            show_other_turns[i].second.current_field = FieldTypes::RIVER;
                            show_other_turns[i].second.river_coordinates.push_back(river_coordinates[j]);
                        }
                    if (active_players_[i].visibility[river_coordinates[river_coordinates.size() - 1].first][river_coordinates[river_coordinates.size() - 1].second]) {
                        show_other_turns[i].second.current_player = PlayerTypes::ALIVE_PLAYER;
                    } else {
                        show_other_turns[i].second.current_player = PlayerTypes::NO_PLAYER;
                    }
                }
            auto tr = std::get<0>(game_map_.GetItems(river_coordinates[river_coordinates.size() - 1].first, river_coordinates[river_coordinates.size() - 1].second));
            active_players_[index].treasure = active_players_[index].treasure || tr;
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, tr);
            active_players_[index].m = river_coordinates[river_coordinates.size() - 1].first;
            active_players_[index].n = river_coordinates[river_coordinates.size() - 1].second;
        } else {
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    for (size_t j = 0; j < river_coordinates.size(); j++)
                        if (active_players_[i].visibility[river_coordinates[j].first][river_coordinates[j].second]) {
                            show_other_turns[i].second.visible_to = true;
                            show_other_turns[i].second.current_field = FieldTypes::RIVER;
                            show_other_turns[i].second.river_coordinates.push_back(river_coordinates[j]);
                        }
                    if (active_players_[i].visibility[river_coordinates[river_coordinates.size() - 1].first][river_coordinates[river_coordinates.size() - 1].second]) {
                        show_other_turns[i].second.current_player = PlayerTypes::DEAD_PLAYER;
                    } else {
                        show_other_turns[i].second.current_player = PlayerTypes::NO_PLAYER;
                    }
                }
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, std::get<0>(game_map_.WatchItems(river_coordinates[river_coordinates.size() - 1].first, river_coordinates[river_coordinates.size() - 1].second)));
            active_players_[index].m = river_coordinates[river_coordinates.size() - 1].first;
            active_players_[index].n = river_coordinates[river_coordinates.size() - 1].second;
        }
        break;
    }
    case FieldTypes::METRO: {
        auto metro_coordinates = game_map_.GetTeleportCoordinates(turn.m, turn.n);
        active_players_[index].visibility[metro_coordinates.first][metro_coordinates.second] = true;
        if (active_players_[index].health > 0) {
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    if (active_players_[i].visibility[metro_coordinates.first][metro_coordinates.second]) {
                        show_other_turns[i].second.metro_m = metro_coordinates.first;
                        show_other_turns[i].second.metro_n = metro_coordinates.second;
                    }
                }
            auto tr = std::get<0>(game_map_.GetItems(turn.m, turn.n)) || std::get<0>(game_map_.GetItems(metro_coordinates.first, metro_coordinates.second));
            active_players_[index].treasure = active_players_[index].treasure || tr;
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, tr);
        } else {
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    if (active_players_[i].visibility[metro_coordinates.first][metro_coordinates.second]) {
                        show_other_turns[i].second.metro_m = metro_coordinates.first;
                        show_other_turns[i].second.metro_n = metro_coordinates.second;
                    }
                }
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, std::get<0>(game_map_.WatchItems(turn.m, turn.n)) || std::get<0>(game_map_.WatchItems(metro_coordinates.first, metro_coordinates.second)));
        }
        active_players_[index].m = metro_coordinates.first;
        active_players_[index].n = metro_coordinates.second;
        break;
    }
    case FieldTypes::ARMOURY: {
        if (active_players_[index].health > 0) {
            // Создание сообщений для ждущих игроков.
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    if (show_other_turns[i].second.visible_to) {
                        show_other_turns[i].second.current_player = PlayerTypes::ALIVE_PLAYER;
                    }
                }
            // Создание сообщения для сходившего игрока.
            auto tr = std::get<0>(game_map_.GetItems(turn.m, turn.n));
            auto wpns = game_map_.GetWeapon(turn.m, turn.n);
            active_players_[index].treasure = active_players_[index].treasure || tr;
            active_players_[index].bombs += std::get<0>(wpns); active_players_[index].ammo += std::get<1>(wpns); active_players_[index].concrete += std::get<2>(wpns);
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, tr);
        } else {
            // Создание сообщений для ждущих игроков.
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    if (show_other_turns[i].second.visible_to) {
                        show_other_turns[i].second.current_player = PlayerTypes::DEAD_PLAYER;
                        show_other_turns[i].second.current_treasure = std::get<0>(game_map_.WatchItems(turn.m, turn.n));
                    }
                }
            // Создание сообщения для сходившего игрока.
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, std::get<0>(game_map_.WatchItems(turn.m, turn.n)));
        }
        active_players_[index].m = turn.m;
        active_players_[index].n = turn.n;
        break;
    }
    case FieldTypes::HOSPITAL: {
        if (active_players_[index].health > 0) {
            // Создание сообщений для ждущих игроков.
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    if (show_other_turns[i].second.visible_to) {
                        show_other_turns[i].second.current_player = PlayerTypes::ALIVE_PLAYER;
                    }
                }
            // Создание сообщения для сходившего игрока.
            auto tr = std::get<0>(game_map_.GetItems(turn.m, turn.n));
            active_players_[index].treasure = active_players_[index].treasure || tr;
            active_players_[index].aids += game_map_.GetMedicine(turn.m, turn.n);
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, tr);
        } else {
            // Создание сообщений для ждущих игроков.
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    if (show_other_turns[i].second.visible_to) {
                        show_other_turns[i].second.current_player = PlayerTypes::DEAD_PLAYER;
                        show_other_turns[i].second.current_treasure = std::get<0>(game_map_.WatchItems(turn.m, turn.n));
                    }
                }
            // Создание сообщения для сходившего игрока.
            active_players_[index].health = 7;
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, std::get<0>(game_map_.WatchItems(turn.m, turn.n)));
        }
        active_players_[index].m = turn.m;
        active_players_[index].n = turn.n;
        break;
    }
    case FieldTypes::BOMB: {
        if (active_players_[index].health > 0) {
            // Создание сообщений для ждущих игроков.
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    show_other_turns[i].second.current_player = PlayerTypes::ALIVE_PLAYER;
                    show_other_turns[i].second.current_treasure = std::get<0>(game_map_.WatchItems(turn.m, turn.n)) || active_players_[index].treasure;
                }
            // Создание сообщения для сходившего игрока.
            if (active_players_[index].treasure) {
                active_players_[index].treasure = false;
                game_map_.PushItems(turn.m, turn.n, std::make_tuple(true, 0, 0, 0));
            }
            active_players_[index].bombs = active_players_[index].ammo = active_players_[index].concrete = active_players_[index].aids = 0;
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, std::get<0>(game_map_.WatchItems(turn.m, turn.n)));
            active_players_[index].health = 0;
            game_map_.ChangeType(turn.m, turn.n, FieldTypes::ROAD);
        } else {
            // Создание сообщений для ждущих игроков.
            for (size_t i = 0; i < show_other_turns.size(); i++)
                if (static_cast<int32_t>(i) != index) {
                    show_other_turns[i].second.current_player = PlayerTypes::DEAD_PLAYER;
                    show_other_turns[i].second.current_treasure = std::get<0>(game_map_.WatchItems(turn.m, turn.n));
                }
            // Создание сообщения для сходившего игрока.
            EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, std::get<0>(game_map_.WatchItems(turn.m, turn.n)));
        }
        active_players_[index].m = turn.m;
        active_players_[index].n = turn.n;
        break;
    }
    case FieldTypes::CONCRETE: {
        // Создание сообщения для сходившего игрока.
        EncodeShowTurn(show_turn, TurnTypes::GO, active_players_[index].m, active_players_[index].n, turn.m, turn.n, false);
        break;
    }
    }
    // Отправка сообщений.
    show_turn.my_turn = (index == static_cast<int32_t>((index + 1) % active_players_.size()));
    show_other_turns.erase(show_other_turns.begin() + index);
   // std::cout << "Размер массива клеточек: " << show_other_turns[0].second.river_coordinates.size() << std::endl;
    auto result = std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].id);
    index = (index + 1) % active_players_.size();
    return result;
}

void Game::EncodeShowTurn(ShowTurn &show_turn, TurnTypes turn, int32_t previous_m, int32_t previous_n, int32_t current_m, int32_t current_n, bool treasure) {
    show_turn.turn = turn;
    show_turn.health = active_players_[index].health;
    show_turn.treasure = treasure;
    show_turn.bombs = active_players_[index].bombs;
    show_turn.ammo = active_players_[index].ammo;
    show_turn.concrete = active_players_[index].concrete;
    show_turn.aids = active_players_[index].aids;
    show_turn.m = current_m;
    show_turn.n = current_n;
    show_turn.field = game_map_.GetType(current_m, current_n);
    show_turn.previous_player = IsHerePlayer(previous_m, previous_n);
    show_turn.current_player = IsHerePlayer(current_m, current_n);
    if (show_turn.field == FieldTypes::METRO)
        std::tie(show_turn.metro_m, show_turn.metro_n) = game_map_.GetTeleportCoordinates(current_m, current_n);
    if (show_turn.field == FieldTypes::RIVER) {
        show_turn.river_coordinates = game_map_.GetRiverCoordinates(current_m, current_n);
        show_turn.current_player = IsHerePlayer(show_turn.river_coordinates[show_turn.river_coordinates.size() - 1].first,
                                                show_turn.river_coordinates[show_turn.river_coordinates.size() - 1].second);
    }
}

PlayerTypes Game::IsHerePlayer(int32_t m, int32_t n) {
    std::cout << std::endl;
    for (size_t i = 0; i < active_players_.size(); i++)
        if ((active_players_[i].m == m) && (active_players_[i].n == n) && (static_cast<int32_t>(i) != index)) {
            if (active_players_[i].health > 0)
                return PlayerTypes::ALIVE_PLAYER;
            else
                return PlayerTypes::DEAD_PLAYER;
        }
    return PlayerTypes::NO_PLAYER;   
}
