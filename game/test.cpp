/* Copyright (C) 2017 Mikhail Masyagin */

/*
  Система юнит-тестов класса Game.
 */

#include <cstring>
#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <cinttypes>
#include <cstdlib>

// Тестирующаяся библиотека.
#include "game.h"

// Прототип карты.
int32_t arr[8][10] = {
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {1,  0,  0,  0,  0,  0,  0,  0,  0,  1},
    {1,  0,  0,  0,  0,  0,  0,  0,  0,  1},
    {1,  0,  0,  0,  0,  0,  0,  0,  0,  1},
    {1,  0,  0,  0,  0,  0,  0,  0,  0,  1},
    {1,  0,  0,  0,  0,  0,  0,  0,  0,  1},
    {1,  0,  0,  0,  0,  0,  0,  0,  0,  1},
    {1,  1,  1,  1,  1,  0,  1,  1,  1,  1},
};

void ShowGraphics(std::vector<std::vector<std::string>> graphics);

Turn Input(int32_t my_m, int32_t my_n);

void ChangeAndDraw(std::vector<std::vector<std::string>> &graphics, int32_t &my_m, int32_t &my_n, int32_t &health, bool &has_treasure, int32_t &my_bombs, int32_t &my_ammo, int32_t &my_aids, ShowTurn show_turn, FieldTypes &type_of_field);

void DrawWatch(std::vector<std::vector<std::string>> &graphics, ShowTurn show_turn);

void DrawGo(std::vector<std::vector<std::string>> &graphics, int32_t &my_m, int32_t &my_n, int32_t &health, bool &has_treasure, int32_t &my_bombs, int32_t &my_ammo, int32_t &my_aids, ShowTurn show_turn, FieldTypes &type_of_field);

int main() {
    // Конвертируем прототип карты int32_t arr[][] в вектор векторов.
    std::vector<std::vector<int32_t>> game_map_scheme;
    game_map_scheme.resize(8);
    for (int32_t i = 0; i < 8; i++) {
        game_map_scheme[i].resize(10);
        for (int32_t j = 0; j < 10; j++)
            game_map_scheme[i][j] = arr[i][j];
    }
    // Создаём игру.
    GameMap game_map = GameMap(game_map_scheme);
    std::vector<ActivePlayer> active_players({ActivePlayer(228, "Asuka Langley")});
    Game game = Game(game_map_scheme, active_players);
    // Игра началась.
    // ****************************************************************************
    // Заменить на работу с сервером.
    auto start = game.StartGame();
    // ****************************************************************************
    
    int32_t my_m = start.first[0].second.m, my_n = start.first[0].second.n;
    FieldTypes type_of_field = FieldTypes::ROAD;
    int32_t health = 5;
    bool has_treasure = false;
    int32_t my_bombs = 0, my_ammo = 0, my_aids = 0;
    std::vector<std::vector<std::string>> graphics;
    graphics.resize(start.first[0].second.size_m);
    for (size_t i = 0; i < graphics.size(); i++) {
        graphics[i].resize(start.first[0].second.size_n);
        for (size_t j = 0; j < graphics[i].size(); j++)
            graphics[i][j] = "*";
    }
    graphics[my_m][my_n] = "@";
    ShowGraphics(graphics);

    //
    while (true) {
        auto turn = Input(my_m, my_n);
        ChangeAndDraw(graphics, my_m, my_n, health, has_treasure, my_bombs, my_ammo, my_aids, std::get<0>(game.TurnHandler(turn)).second, type_of_field);
        std::cout << "gregeth4thrthrhrthbrynrthnrgntrynrenrtnrtn" << std::endl;
    }
    return 0;
}

void ShowGraphics(std::vector<std::vector<std::string>> graphics) {
    // std::cout << "\033[2J\033[1;1H";
    for (size_t i = 0; i < graphics.size(); i++) {
        for (size_t j = 0; j < graphics[i].size(); j++)
            std::cout << graphics[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Turn Input(int32_t my_m, int32_t my_n) {
    Turn turn;
    std::cout << "command: ";
    std::string command;
    std::cin >> command;
    if (command == "go") {
        turn.type_of_turn = TurnTypes::GO;
        std::cout << "direction: ";
        std::string direction;
        std::cin >> direction;
        if (direction == "up") {
            turn.m = my_m - 1;
            turn.n = my_n;
            return turn;
        }
        if (direction == "down") {
            turn.m = my_m + 1;
            turn.n = my_n;
            return turn;
        }
        if (direction == "left") {
            turn.m = my_m;
            turn.n = my_n - 1;
            return turn;
        }
        if (direction == "right") {
            turn.m = my_m;
            turn.n = my_n + 1;
            return turn;
        }
    }
    return turn;
}

void ChangeAndDraw(std::vector<std::vector<std::string>> &graphics, int32_t &my_m, int32_t &my_n, int32_t &health, bool &has_treasure, int32_t &my_bombs, int32_t &my_ammo, int32_t &my_aids, ShowTurn show_turn, FieldTypes &type_of_field) {
    if (show_turn.type_of_error != ErrorTypes::NO_ERROR) {
        std::cout << "Error!!! gewgewgegrgerger" << std::endl;
        return; 
    }
    switch (show_turn.type_of_turn) {
    case TurnTypes::GO: {
        DrawGo(graphics, my_m, my_n, health, has_treasure, my_bombs, my_ammo, my_aids, show_turn, type_of_field);
        break;
    }
    default: {
        std::cout << "ERROR, UNKNOWN COMMAND!!!" << std::endl;
        exit(1);
    }
    }
}

void DrawGo(std::vector<std::vector<std::string>> &graphics, int32_t &my_m, int32_t &my_n, int32_t &health, bool &has_treasure, int32_t &my_bombs, int32_t &my_ammo, int32_t &my_aids, ShowTurn show_turn, FieldTypes &type_of_field) {
    switch (show_turn.type_of_field) {
    case FieldTypes::ROAD: {
        switch (type_of_field) {
        case FieldTypes::ROAD: {
            graphics[my_m][my_n] = " ";
            break;
        }
        case FieldTypes::WALL: {
            graphics[my_m][my_n] = "█";
            break;
        }
        }
        my_m = show_turn.m;
        my_n = show_turn.n;
        type_of_field = show_turn.type_of_field;
        health = show_turn.health;
        graphics[my_m][my_n] = "@";
        break;
    }
    case FieldTypes::WALL: {
        graphics[show_turn.m][show_turn.n] = "█";
        break;
    }
    }
    ShowGraphics(graphics);
}
