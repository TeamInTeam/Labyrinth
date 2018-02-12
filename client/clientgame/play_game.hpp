// Created by Vladislav Melnikov on 19.12.17.

#ifndef LABTECH_PLAY_GAME_HPP
#define LABTECH_PLAY_GAME_HPP

#include "GameGUI.hpp"

struct Info {
    bool flag;
    std::string name;
    int32_t count;
};

void start_game(Info info);


#endif //LABTECH_PLAY_GAME_HPP
