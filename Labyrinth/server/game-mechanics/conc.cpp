/* Copyright (C) 2017 Mikhail Masyagin */

#include <cstring>
#include <cinttypes>
#include <utility>
#include <tuple>
#include "game.h"
#include <iostream>

/*
std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::ConcreteHandler(Turn turn) {
    ShowTurn show_turn;
    show_turn.turn = TurnTypes::CONCRETE;
    std::vector<std::pair<int32_t, ShowOtherTurn>> show_other_turns;

    if (!Check(show_turn, turn.m, turn.n, TurnTypes::CONCRETE)) {
        show_turn.my_turn = true;
        return std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[index].id);
    }

    // Отображение для других игроков.
    for (size_t i = 0; i < active_players_.size(); i++)
        if (static_cast<int32_t>(i) != index) {
            ShowOtherTurn show_other_turn;
            show_other_turn.turn = TurnTypes::CONCRETE;

            if (active_players_[i].visibility[active_players_[index].m][active_players_[index].n]) {
                
            }
            if (active_players_[i].visibility[turn.m][turn.n]) {
                
            }
            
            show_other_turn.my_turn = (i == (index + 1) % active_players_.size());
            show_other_turns.push_back(std::make_pair(active_players_[i].id, show_other_turn));
        }

    active_players_[index].visibility[turn.m][turn.n] = true;
    auto type_of_field = game_map_.GetType(turn.m, turn.n);
    auto sizes = game_map_.GetSize();
    if ((type_of_field == FieldTypes::ROAD) && !(IsHerePlayer(turn.m, turn.n)) && !((turn.m == 0) || (turn.n == 0) || (turn.m == sizes.first - 1) || (turn.n == sizes.second - 1))) {
        active_players_[index].concrete--;
        game_map_.ChangeType(turn.m, turn.n, FieldTypes::CONCRETE);
    }
    Encode(show_turn, turn.m, turn.n, false);
    
    auto result = std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].id);
    index = (index + 1) % active_players_.size();
    return result;
}
*/
