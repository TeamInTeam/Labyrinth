/* Copyright (C) 2017 Mikhail Masyagin */

#include <vector>
#include <utility>
#include <map>
#include <cinttypes>
#include <ctime>
#include <boost/variant.hpp>
#include "gamemap.h"

#include <iostream>

GameMap::GameMap(std::vector<std::vector<int32_t> > game_map_scheme) {
    size_m_ = game_map_scheme.size();
    size_n_ = game_map_scheme[0].size();

    game_map_.resize(size_m_);
    for (int32_t i = 0; i < size_m_; i++)
        game_map_[i].resize(size_n_);
    
    std::srand(unsigned(std::time(0)));
    
    for (int32_t i = 0; i < size_m_; i++)
        for (int32_t j = 0; j < size_n_; j++) {
            int32_t type_of_field = game_map_scheme[i][j] % 10;
            switch (type_of_field) {
            case 0: {
                game_map_[i][j] = Road();
                break;
            }
            case 1: {
                if ((i == 0) || (j == 0) || (i == size_m_ - 1) || (j == size_n_ - 1))
                    game_map_[i][j] = Wall(false);
                else {
                    game_map_[i][j] = Wall(true);
                }
                break;
            }
            case 2: {
                std::vector<std::pair<int32_t, int32_t> > coordinates;
                int32_t coordinate_m = i, coordinate_n = j;
                coordinates.push_back(std::make_pair(i, j));
                while (true) {
                    if (game_map_scheme[coordinate_m + 1][coordinate_n] == 2) {
                        coordinates.push_back(std::make_pair(coordinate_m + 1, coordinate_n));
                        coordinate_m++;
                    } else if (game_map_scheme[coordinate_m][coordinate_n - 1] == 2) {
                        coordinates.push_back(std::make_pair(coordinate_m, coordinate_n - 1));
                        coordinate_n--;
                    } else {
                        break;
                    }
                }
                game_map_[i][j] = River(coordinates);
                break;
            }
            case 3: {
                Metro metro(game_map_scheme[i][j]);
                for (int32_t k = 0; k < size_m_; k++)
                    for (int32_t l = 0; l < size_n_; l++)
                        if ((game_map_scheme[k][l] == game_map_scheme[i][j]) && ((i != k) || (j != l))) {
                            metro.teleport_m = k;
                            metro.teleport_n = l;
                            game_map_[i][j] = metro;
                            break;
                        }
                break;
            }
            case 4: {
                game_map_[i][j] = Armoury(int32_t(std::rand() % 50 + 1), int32_t(std::rand() % 50 + 1));
                break;
            }
            case 5: {
                game_map_[i][j] = Hospital(int32_t(std::rand() % 50 + 1));
                break;
            }
            case 6: {
                game_map_[i][j] = Mine();
                break;
            }
            case 7: {
                game_map_[i][j] = Concrete();
                break;
            }
            }
        }
}

std::pair<int32_t, int32_t> GameMap::GetSize() {
    return std::make_pair(size_m_, size_n_);
}

FieldTypes GameMap::GetType(int32_t m, int32_t n) {
    return boost::apply_visitor(game_map_visitor::GetType(), game_map_[m][n]);
}

void GameMap::ChangeType(int32_t m, int32_t n, FieldTypes type_of_field) {
    auto items = GetItems(m, n);
    switch (type_of_field) {
    case FieldTypes::ROAD: {
        game_map_[m][n] = Road();
        PushItems(m, n, items);
        break;
    }
    case FieldTypes::MINE: {
        game_map_[m][n] = Mine();
        PushItems(m, n, items);
        break;
    }
    case FieldTypes::CONCRETE: {
        game_map_[m][n] = Concrete();
        PushItems(m, n, items);
        break;
    }
    default: {
        break;
    }
    }
}

std::tuple<bool, int32_t, int32_t, int32_t> GameMap::WatchItems(int32_t m, int32_t n) {
    return  boost::apply_visitor(game_map_visitor::WatchItems(), game_map_[m][n]);
}

std::tuple<bool, int32_t, int32_t, int32_t> GameMap::GetItems(int32_t m, int32_t n) {
    auto result = boost::apply_visitor(game_map_visitor::WatchItems(), game_map_[m][n]);
    auto tp = std::make_tuple(false, 0, 0, 0);
    boost::apply_visitor([tp](auto &aut) {game_map_visitor::PushItems()(aut, tp); }, game_map_[m][n]);
    return result;
}

void GameMap::PushItems(int32_t m, int32_t n, std::tuple<bool, int32_t, int32_t, int32_t> tp) {
    boost::apply_visitor([tp](auto &aut) {game_map_visitor::PushItems()(aut, tp); }, game_map_[m][n]);
}

bool GameMap::GetMedicine(int32_t m, int32_t n) {
    return boost::apply_visitor(game_map_visitor::GetMedicine(), game_map_[m][n]);
}

std::pair<int32_t, int32_t> GameMap::GetWeapon(int32_t m, int32_t n) {
    return boost::apply_visitor(game_map_visitor::GetWeapon(), game_map_[m][n]);
}

std::pair<int32_t, int32_t> GameMap::GetTeleportCoordinates(int32_t m, int32_t n) const {
    return boost::apply_visitor(game_map_visitor::GetTeleportCoordinates(), game_map_[m][n]);
}

std::vector<std::pair<int32_t, int32_t>> GameMap::GetRiverCoordinates(int32_t m, int32_t n) const {
    return boost::apply_visitor(game_map_visitor::GetRiverCoordinates(), game_map_[m][n]);
}

bool GameMap::IsDestroyable(int32_t m, int32_t n) const {
    return boost::apply_visitor(game_map_visitor::IsDestroyable(), game_map_[m][n]);
}
