/* Copyright (C) 2017 Mikhail Masyagin */

#include <cstring>
#include <cinttypes>
#include <utility>
#include "game.h"

// WatchHandler вовзращает сообщения сходившему игроку, ждущим игрокам, а также индекс вновь ходящего игрока. 
std::tuple<std::pair<int32_t, ShowTurn>, std::vector<std::pair<int32_t, ShowOtherTurn>>, int32_t> Game::WatchHandler(Turn turn) {
    // Создание дефолтных исходящих сообщний.
    ShowTurn show_turn;
    show_turn.turn = TurnTypes::WATCH;
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

    // Добавление очередной клетки в поле видимости игрока.
    active_players_[index].visibility[turn.m][turn.n] = true;
    
    // Создание сообщений для ждущих игроков.
    for (size_t i = 0; i < active_players_.size(); i++)
        if (static_cast<int32_t>(i) != index) {
            ShowOtherTurn show_other_turn;
            show_other_turn.turn = TurnTypes::WATCH;
            show_other_turn.my_turn = (i == (index + 1) % active_players_.size());
            show_other_turns.push_back(std::make_pair(active_players_[i].id, show_other_turn));
        }

    // Создание сообщения для сходившего игрока.
    show_turn.treasure = std::get<0>(game_map_.WatchItems(turn.m, turn.n));
    show_turn.m = turn.m;
    show_turn.n = turn.n;
    show_turn.field = game_map_.GetType(turn.m, turn.n);
    show_turn.current_player = IsHerePlayer(turn.m, turn.n);

    // Отправка сообщений.
    show_turn.my_turn = (index == static_cast<int32_t>((index + 1) % active_players_.size()));
    auto result = std::make_tuple(std::make_pair(active_players_[index].id, show_turn), show_other_turns, active_players_[(index + 1) % active_players_.size()].id);
    index = (index + 1) % active_players_.size();
    return result;
}
