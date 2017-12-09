// Created by Vladislav Melnikov on 07.12.17.

#include "MenuItem.hpp"

MenuItem::MenuItem(MenuType type, std::string str) {
    this->type = type;
    name = str;
}

MenuItem::MenuItem(MenuType type, std::string str, MENU** dest, MENU* gui) {
    this->type = type;
    name = str;
    this->dest = dest;
    this->gui = gui;
}


//////////////////////////////////////////
//int32_t arr[12][12] = {
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
//};
int32_t arr[12][12] = {
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
        {1,  0,  0,  0,  0,  1,  0,  1,  0,  0, 23,  1},
        {1, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1},
        {1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1},
        {0,  1,  0,  0,  1,  0,  0,  0,  0,  2,  1,  1},
        {1,  0,  0,  0,  1,  0,  0,  0,  0,  2,  0,  1},
        {1,  0,  0,  0,  0,  0,  0,  0,  2,  2,  0,  1},
        {1,  1,  1,  1,  1,  0,  1,  1,  2,  1,  0,  1},
        {1, 13,  0,  0,  1,  0,  1,  0,  2,  0,  0,  1},
        {0,  1,  0,  0,  0,  0,  2,  2,  2,  0,  1,  1},
        {1,  0,  0,  0,  1,  0,  2,  0,  0, 23,  0,  1},
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
};
//////////////////////////////////////////


// !!!!!!!!!!!!ВЫНЕСТИ В ДРУГОЙ ФАЙЛ!!!!!!!!!!!!
void start_game() {
    /////////////////////////////////
    // Конвертируем прототип карты int32_t arr[][] в вектор векторов.
    std::vector<std::vector<int32_t>> game_map_scheme;
    game_map_scheme.resize(12);
    for (int32_t i = 0; i < 12; i++) {
        game_map_scheme[i].resize(12);
        for (int32_t j = 0; j < 12; j++)
            game_map_scheme[i][j] = arr[i][j];
    }
    // Создаём игру.
    GameMap game_map = GameMap(game_map_scheme);
    std::vector<ActivePlayer> active_players({ActivePlayer(228, "Asuka Langley")});
    Game game_server = Game(game_map_scheme, active_players);
//    // ****************************************************************************
//    // Заменить на работу с сервером.
    ShowTurn start_game = game_server.StartGame().first[0].second;
//    // ****************************************************************************
    /////////////////////////////////
    GameGUIw game_gui(start_game);

    while (true) {
        auto player_turn = game_gui.send_request();
        if (player_turn.type_of_turn == TurnTypes::END)
            break;
        auto result = game_server.TurnHandler(player_turn);
        game_gui.get_response(std::get<0>(result).second);
    }
}

bool MenuItem::action() {
    switch (type) {
        case MenuType::LINK:
            *dest = gui;
            break;
        case MenuType::START:
            start_game();
            break;
        case MenuType::QUIT:
            return false;
        case MenuType::BUNG:
            break;
    }
    return true;
}