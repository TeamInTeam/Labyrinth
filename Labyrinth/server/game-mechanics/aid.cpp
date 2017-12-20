/* Copyright (C) 2017 Mikhail Masyagin */

#include <cstring>
#include <cinttypes>
#include <utility>
#include <tuple>
#include "game.h"

/*
std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::AidHandler(Turn) {
    ShowTurn show_turn;
    show_turn.turn = TurnTypes::AID;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;
    
    if (!Check(show_turn, 0, 0, TurnTypes::AID)) {
        show_turn.my_turn = true;
        return std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[index].id);
    }

    // Отображение для других игроков.
    for (size_t i = 0; i < active_players_.size(); i++)
        if (static_cast<int32_t>(i) != index) {
            ShowOtherTurn show_other_turn;
            show_other_turn.turn = TurnTypes::AID;
            show_other_turn.my_turn = (i == (index + 1) % active_players_.size());
            show_other_turns.push_back(std::make_pair(active_players_[i].id, show_other_turn));
        }
    
    if (active_players_[index].health < 7) {
        active_players_[index].health++;
        active_players_[index].aids--;
    }

    Encode(show_turn, 0, 0, false);

    // Отображение для других игроков.
    
    auto result = std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].id);
    index = (index + 1) % active_players_.size();
    return result;
}
*/
