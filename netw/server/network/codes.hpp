/* Copyright (C) 2017 Roman Koshelev */
#pragma once
#include <type_traits>
#include <utility>
#include "../game-mechanics/game.h"

struct Start_room
{
	Start_room() = default;
	Start_room(const uint32_t count_players, const char* room_name)
		: count_players_(count_players)
	{
		room_name_[0] = '\0';
		strcat(room_name_, room_name);
	}
	uint32_t count_players_;
	char room_name_[32];
};

struct join_room {
	std::uint32_t id_;
};

struct get_list_room {
};

struct list_room_package {
	std::vector<std::pair<std::int32_t, std::string>> vec;
};

REGISTER_TYPE_1(ShowTurn, 10)
REGISTER_TYPE_1(ShowOtherTurn, 11)
REGISTER_TYPE_1(Turn, 12)
REGISTER_TYPE_1(Start_room, 13)
REGISTER_TYPE_1(join_room, 14)
REGISTER_TYPE_1(get_list_room, 14)
REGISTER_TYPE_1(list_room_package, 15)

inline void memcpy_fl(void * Lhs, void* Rhs, const std::size_t size, const bool fl) {
	if (fl) {
		std::memcpy(Lhs, Rhs, size);
	} else {
		std::memcpy(Rhs, Lhs, size);
	}
}

inline std::size_t codeStr(char* mem, std::string& str, bool fl) {
	std::size_t ch = 0;
	std::uint32_t sizeStr = str.size() + 1;
	memcpy_fl(mem + ch, &sizeStr, sizeof(std::uint32_t), fl);
	ch += sizeof(std::uint32_t);
	if (fl) {
		std::memcpy(mem + ch, str.c_str(), sizeStr);
		ch += sizeStr;
	} else {
		char buffer[32];
		std::memcpy(buffer, mem + ch, sizeStr);
		ch += sizeStr;
		str = { buffer,sizeStr };
	}
	return ch;
}

inline std::size_t code_vec_pair_int_str(char* mem, std::vector<std::pair<std::int32_t, std::string>> &vec, bool fl) {
	std::size_t ch = 0;
	std::uint32_t size = vec.size();
	memcpy_fl(mem + ch, &size, sizeof(std::uint32_t), fl);
	vec.reserve(size);
	ch += sizeof(std::uint32_t);
	if (fl) {
		for (auto i : vec) {
			std::uint32_t id = i.first;
			std::memcpy(mem + ch, &id, sizeof(std::uint32_t));
			ch += sizeof(std::uint32_t);

			ch += codeStr(mem + ch, i.second, true);
		}
	} else {
		for (std::size_t i = 0; i < size; ++i) {
			std::uint32_t id = 0;
			std::memcpy(&id, mem + ch, sizeof(std::uint32_t));
			ch += sizeof(std::uint32_t);

			std::pair<std::uint32_t, std::string> tmp;
			tmp.first = id;

			std::string buf;
			ch += codeStr(mem + ch, buf, false);

			tmp.second = buf;
			vec.push_back(tmp);

		}
	}
	return ch;
}

inline std::size_t  binary_size_vec_pair_int_str(const std::vector<std::pair<std::int32_t, std::string>> &vec) {
	std::size_t size = sizeof(std::uint32_t) + vec.size() * sizeof(std::uint32_t) * 2;
	for (auto i : vec) {
		size += i.second.size() + 1;
	}
	return size;
}

inline std::size_t  binary_size(list_room_package& data) {
	return binary_size_vec_pair_int_str(data.vec);
}

inline void  encodeX(char* mem, list_room_package& data) {
	code_vec_pair_int_str(mem, data.vec, true);
}

inline void  decodeX(char* mem, list_room_package& data) {
	code_vec_pair_int_str(mem, data.vec, false);
}

inline void  code(char* mem, ShowOtherTurn& data, bool fl) {
	std::size_t ch = 0;
	memcpy_fl(mem + ch, &data.visible_from, sizeof(data.visible_from), fl);
	ch += sizeof(data.visible_from);
	memcpy_fl(mem + ch, &data.turn, sizeof(data.turn), fl);
	ch += sizeof(data.turn);
	memcpy_fl(mem + ch, &data.previous_m, sizeof(data.previous_m), fl);
	ch += sizeof(data.previous_m);
	memcpy_fl(mem + ch, &data.previous_n, sizeof(data.previous_n), fl);
	ch += sizeof(data.previous_n);
	memcpy_fl(mem + ch, &data.previous_field, sizeof(data.previous_field), fl);
	ch += sizeof(data.previous_field);
	memcpy_fl(mem + ch, &data.previous_treasure, sizeof(data.previous_treasure), fl);
	ch += sizeof(data.previous_treasure);
	memcpy_fl(mem + ch, &data.metro_m, sizeof(data.metro_m), fl);
	ch += sizeof(data.metro_m);
	memcpy_fl(mem + ch, &data.metro_n, sizeof(data.metro_n), fl);
	ch += sizeof(data.metro_n);

	std::uint32_t size = data.river_coordinates.size();
	memcpy_fl(mem + ch, &size, sizeof(std::uint32_t), fl);
	data.river_coordinates.resize(size);
	ch += sizeof(std::uint32_t);
	memcpy_fl(mem + ch, data.river_coordinates.data(), size * sizeof(std::pair<int32_t, int32_t>), fl);
	ch += size * sizeof(std::pair<int32_t, int32_t>);
	
	
	memcpy_fl(mem + ch, &data.previous_player, sizeof(data.previous_player), fl);
	ch += sizeof(data.previous_player);
	memcpy_fl(mem + ch, &data.current_player, sizeof(data.current_player), fl);
	ch += sizeof(data.current_player);

	memcpy_fl(mem + ch, &data.current_treasure, sizeof(data.current_treasure), fl);
	ch += sizeof(data.current_treasure);

	memcpy_fl(mem + ch, &data.health, sizeof(data.health), fl);
	ch += sizeof(data.health);
	memcpy_fl(mem + ch, &data.treasure, sizeof(data.treasure), fl);
	ch += sizeof(data.treasure);
	memcpy_fl(mem + ch, &data.visible_to, sizeof(data.visible_to), fl);
	ch += sizeof(data.visible_to);
	memcpy_fl(mem + ch, &data.current_m, sizeof(data.current_m), fl);
	ch += sizeof(data.current_m);
	memcpy_fl(mem + ch, &data.current_n, sizeof(data.current_n), fl);
	ch += sizeof(data.current_n);
	memcpy_fl(mem + ch, &data.current_field, sizeof(data.current_field), fl);
	ch += sizeof(data.current_field);

	memcpy_fl(mem + ch, &data.my_turn, sizeof(data.my_turn), fl);
	ch += sizeof(data.my_turn);
	memcpy_fl(mem + ch, &data.game_end, sizeof(data.game_end), fl);
	ch += sizeof(data.game_end);
}

inline std::size_t  binary_size(ShowOtherTurn& data) {
	return sizeof(data.visible_from) + sizeof(data.turn) +
		sizeof(data.previous_m) + sizeof(data.previous_n) + sizeof(data.previous_field) +
		sizeof(data.metro_m) + sizeof(data.metro_n) + sizeof(std::uint32_t) +
		data.river_coordinates.size() * sizeof(std::pair<int32_t, int32_t>) +
		sizeof(data.previous_treasure) + sizeof(data.current_player) +
		sizeof(data.health) + sizeof(data.treasure) + sizeof(data.previous_player) +
		sizeof(data.visible_to) + sizeof(data.current_m) + sizeof(data.current_n) +
		sizeof(data.my_turn) + sizeof(data.game_end) + sizeof(data.current_field) +
		sizeof(data.current_treasure);

}

inline void  encodeX(char* mem, ShowOtherTurn& data) {
	code(mem, data, true);
}

inline void decodeX(char* mem, ShowOtherTurn& data) {
	code(mem, data, false);
}


inline void  code(char* mem, ShowTurn& data, bool fl) {
	std::size_t ch = 0;
	memcpy_fl(mem + ch, &data.error, sizeof(data.error), fl);
	ch += sizeof(data.error);
	memcpy_fl(mem + ch, &data.turn, sizeof(data.turn), fl);
	ch += sizeof(data.turn);
	memcpy_fl(mem + ch, &data.size_m, sizeof(data.size_m), fl);
	ch += sizeof(data.size_m);
	memcpy_fl(mem + ch, &data.size_n, sizeof(data.size_n), fl);
	ch += sizeof(data.size_n);
	memcpy_fl(mem + ch, &data.m, sizeof(data.m), fl);
	ch += sizeof(data.m);
	memcpy_fl(mem + ch, &data.n, sizeof(data.n), fl);
	ch += sizeof(data.n);
	memcpy_fl(mem + ch, &data.metro_m, sizeof(data.metro_m), fl);
	ch += sizeof(data.metro_m);
	memcpy_fl(mem + ch, &data.metro_n, sizeof(data.metro_n), fl);
	ch += sizeof(data.metro_n);

	std::uint32_t size = data.river_coordinates.size();
	memcpy_fl(mem + ch, &size, sizeof(std::uint32_t), fl);
	data.river_coordinates.resize(size);
	ch += sizeof(std::uint32_t);
	memcpy_fl(mem + ch, data.river_coordinates.data(), size * sizeof(std::pair<int32_t, int32_t>), fl);
	ch += size * sizeof(std::pair<int32_t, int32_t>);

	memcpy_fl(mem + ch, &data.field, sizeof(data.field), fl);
	ch += sizeof(data.field);
	memcpy_fl(mem + ch, &data.current_player, sizeof(data.current_player), fl);
	ch += sizeof(data.current_player);
	memcpy_fl(mem + ch, &data.previous_player, sizeof(data.previous_player), fl);
	ch += sizeof(data.previous_player);

	memcpy_fl(mem + ch, &data.health, sizeof(data.health), fl);
	ch += sizeof(data.health);
	memcpy_fl(mem + ch, &data.treasure, sizeof(data.treasure), fl);
	ch += sizeof(data.treasure);
	memcpy_fl(mem + ch, &data.bombs, sizeof(data.bombs), fl);
	ch += sizeof(data.bombs);
	memcpy_fl(mem + ch, &data.ammo, sizeof(data.ammo), fl);
	ch += sizeof(data.ammo);
	memcpy_fl(mem + ch, &data.concrete, sizeof(data.concrete), fl);
	ch += sizeof(data.concrete);
	memcpy_fl(mem + ch, &data.aids, sizeof(data.aids), fl);
	ch += sizeof(data.aids);

	memcpy_fl(mem + ch, &data.my_turn, sizeof(data.my_turn), fl);
	ch += sizeof(data.my_turn);
	memcpy_fl(mem + ch, &data.game_end, sizeof(data.game_end), fl);
	ch += sizeof(data.game_end);
	memcpy_fl(mem + ch, &data.my_id, sizeof(data.my_id), fl);
	ch += sizeof(data.my_id);

	ch += code_vec_pair_int_str(mem, data.other_players, fl);
	//other_players
}

inline void  encodeX(char* mem, ShowTurn& data) {
	code(mem, data, true);
}

inline void decodeX(char* mem, ShowTurn& data) {
	code(mem, data, false);
}

inline std::size_t  binary_size(ShowTurn& data) {
	return sizeof(data.error) + sizeof(data.turn) +
		sizeof(data.size_m) + sizeof(data.size_n) + sizeof(data.m) + sizeof(data.n) +
		sizeof(data.metro_m) + sizeof(data.metro_n) + sizeof(std::uint32_t) +
		data.river_coordinates.size() * sizeof(std::pair<int32_t, int32_t>) +
		sizeof(data.field) + sizeof(data.current_player) + sizeof(data.previous_player) +
		sizeof(data.health) + sizeof(data.treasure) + sizeof(data.bombs) +
		sizeof(data.ammo) + sizeof(data.concrete) + sizeof(data.aids) +
		sizeof(data.my_turn) + sizeof(data.game_end) + sizeof(data.my_id) +
		binary_size_vec_pair_int_str(data.other_players);

}

template<class Ty>
std::enable_if_t<std::is_trivially_copyable<Ty>::value, void>  encodeX(char* mem, Ty& data) {
	std::memcpy(mem, reinterpret_cast<char*>(&data), sizeof(Ty));
}

template<class Ty>
std::enable_if_t<std::is_trivially_copyable<Ty>::value, void>  decodeX(char* mem, Ty& data) {
	std::memcpy(reinterpret_cast<char*>(&data), mem, sizeof(Ty));
}

template<class Ty>
std::enable_if_t<std::is_trivially_copyable<Ty>::value, std::size_t>  binary_size(Ty& data) {
	return sizeof(Ty);
}