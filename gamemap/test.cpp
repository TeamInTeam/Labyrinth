/* Copyright (C) 2017 Mikhail Masyagin */

/*
  Система юнит-тестов класса GameMap.
 */

#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <cinttypes>

// Тестирующаяся библиотека.
#include "gamemap.h"

// Прототип карты.
int32_t arr[8][10] = {
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  0,  0,  0,  0, 13,  0, 23,  2,  1},
    {1,  0,  0,  0,  0,  0,  0,  0,  2,  1},
    {1,  0,  0, 13,  0,  0,  1,  0,  2,  1},
    {0,  1,  0,  2,  0,  0,  6,  0,  0,  0},
    {1,  0,  0,  2,  0,  0,  7,  4,  4,  1},
    {1, 23,  2,  2,  0,  7,  7,  5,  5,  1},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
};

int main() {
    // Конвертируем прототип карты int32_t arr[][] в вектор векторов.
    int32_t m = 8, n = 10;
    std::vector<std::vector<int32_t>> game_map_scheme;
    game_map_scheme.resize(m);
    for (int32_t i = 0; i < m; i++) {
        game_map_scheme[i].resize(n);
        for (int32_t j = 0; j < n; j++)
            game_map_scheme[i][j] = arr[i][j];
    }
    // Создаём карту.
    std::cout << "1) Проверка конструктора карты:" << std::endl;
    GameMap game_map = GameMap(game_map_scheme);
    
    // Выводим её размер.
    std::cout << "2) Проверка размеров карты:" << std::endl;
    std::pair<int32_t, int32_t> sizes = game_map.GetSize();
    std::cout << "Size [" << sizes.first << ", " << sizes.second << "]." << std::endl;
    
    // Выводим все её типы.
    std::cout << "3) Проверка анализа типов карты:" << std::endl;
    for (int32_t i = 0; i < m; i++) {
        for (int32_t j = 0; j < n; j++) {
            auto type_of_field = game_map.GetType(i, j);
            switch (type_of_field) {
            case FieldTypes::ROAD: {
                std::cout << "    ROAD ";
                break;
            }
            case FieldTypes::WALL: {
                std::cout << "    WALL ";
                break;
            }
            case FieldTypes::RIVER: {
                std::cout << "   RIVER ";
                break;
            }
            case FieldTypes::METRO: {
                std::cout << "   METRO ";
                break;
            }
            case FieldTypes::ARMOURY: {
                std::cout << " ARMOURY ";
                break;
            }
            case FieldTypes::HOSPITAL: {
                std::cout << "HOSPITAL ";
                break;
            }
            case FieldTypes::MINE: {
                std::cout << "    MINE ";
                break;
            }
            case FieldTypes::CONCRETE: {
                std::cout << "CONCRETE ";
                break;
            }
            }
        }
        std::cout << std::endl;
    }

    // Используем спец-свойства клеток.
    // Стена.
    std::cout << "4) Проверка свойств стен:" << std::endl;
    for (int32_t i = 0; i < m; i++) {
        for (int32_t j = 0; j < n; j++)
            std::cout << "       " << game_map.IsDestroyable(i, j) << " ";
        std::cout << std::endl;
    }

    // Река.
    std::cout << "5) Проверка свойств реки:" << std::endl;
    for (int32_t i = 0; i < m; i++) {
        for (int32_t j = 0; j < n; j++) {
            if (game_map.GetType(i, j) == FieldTypes::RIVER) {
                std::cout << "{";
                auto coordinates = game_map.GetRiverCoordinates(i, j);
                for (size_t i = 0; i < coordinates.size(); i++) {
                    std::cout << "(" << coordinates[i].first << ", " << coordinates[i].second << "), ";
                }
                std::cout << "}" << std::endl;
            }
        }
    }

    // Метро.
    std::cout << "6) Проверка свойств метро:" << std::endl;
    for (int32_t i = 0; i < m; i++) {
        for (int32_t j = 0; j < n; j++) {
            if (game_map.GetType(i, j) == FieldTypes::METRO) {
                auto coordinates = game_map.GetTeleportCoordinates(i, j);
                std::cout << "From [" << i << ", " << j << "] To [" << coordinates.first << ", " << coordinates.second << "]" << std::endl;
            }
        }
    }

    // Замена полей.
    std::cout << "7) Проверка замены полей:" << std::endl;
    game_map.ChangeType(1, 1, FieldTypes::MINE);
    game_map.ChangeType(1, 2, FieldTypes::MINE);
    game_map.ChangeType(2, 1, FieldTypes::MINE);
    game_map.ChangeType(2, 2, FieldTypes::MINE);

    game_map.ChangeType(1, 3, FieldTypes::CONCRETE);
    game_map.ChangeType(1, 4, FieldTypes::CONCRETE);
    game_map.ChangeType(3, 1, FieldTypes::CONCRETE);
    game_map.ChangeType(3, 2, FieldTypes::CONCRETE);

    game_map.ChangeType(4, 1, FieldTypes::ROAD);

    for (int32_t i = 0; i < m; i++) {
        for (int32_t j = 0; j < n; j++) {
            auto type_of_field = game_map.GetType(i, j);
            switch (type_of_field) {
            case FieldTypes::ROAD: {
                std::cout << "    ROAD ";
                break;
            }
            case FieldTypes::WALL: {
                std::cout << "    WALL ";
                break;
            }
            case FieldTypes::RIVER: {
                std::cout << "   RIVER ";
                break;
            }
            case FieldTypes::METRO: {
                std::cout << "   METRO ";
                break;
            }
            case FieldTypes::ARMOURY: {
                std::cout << " ARMOURY ";
                break;
            }
            case FieldTypes::HOSPITAL: {
                std::cout << "HOSPITAL ";
                break;
            }
            case FieldTypes::MINE: {
                std::cout << "    MINE ";
                break;
            }
            case FieldTypes::CONCRETE: {
                std::cout << "CONCRETE ";
                break;
            }
            }
        }
        std::cout << std::endl;
    }

    for (int32_t i = 0; i < m; i++) {
        for (int32_t j = 0; j < n; j++)
            std::cout << "       " << game_map.IsDestroyable(i, j) << " ";
        std::cout << std::endl;
    }
    
    return 0;
}
