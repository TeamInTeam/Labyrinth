/* Copyright (C) 2017 Mikhail Masyagin */

#include <cstring>
#include <cinttypes>
#include <utility>
#include <tuple>
#include "game.h"
#include <iostream>

/*
std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::KnifeHandler(Turn turn) {
    ShowTurn show_turn;
    show_turn.turn = TurnTypes::KNIFE;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;

    if (!Check(show_turn, turn.m, turn.n, TurnTypes::KNIFE)) {
        show_turn.my_turn = true;
        return std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[index].id);
    }

    // Отображение для других игроков.
    for (size_t i = 0; i < active_players_.size(); i++)
        if (static_cast<int32_t>(i) != index) {
            ShowOtherTurn show_other_turn;
            show_other_turn.turn = TurnTypes::KNIFE;

            if (active_players_[i].visibility[active_players_[index].m][active_players_[index].n]) {
                
            }
            if (active_players_[i].visibility[turn.m][turn.n]) {
                
            }

            show_other_turn.my_turn = (i == (index + 1) % active_players_.size());
            show_other_turns.push_back(std::make_pair(active_players_[i].id, show_other_turn));
        }

    Encode(show_turn, turn.m, turn.n, false);
    
    auto result = std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].id);
    index = (index + 1) % active_players_.size();
    return result;
}
*/
