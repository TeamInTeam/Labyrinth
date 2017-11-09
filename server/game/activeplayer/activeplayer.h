/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_ACTIVEPLAYER_ACTIVEPLAYER_H_
#define SERVER_GAME_ACTIVEPLAYER_ACTIVEPLAYER_H_

#include <inttypes.h>
#include <string>
#include <vector>

/*
  Active Player Class.
 */
class ActivePlayer {
 public:
    ActivePlayer(uint64_t game_id,
                 uint64_t player_id,
                 std::string nickname,
                 std::string race,
                 int32_t health,
                 int32_t protection,
                 int32_t damage,
                 int32_t perception,
                 int32_t intellect,
                 int32_t fortune,
                 // Aids and Weapons.
                 int32_t size_x,
                 int32_t size_y,
                 int32_t current_x,
                 int32_t current_y);
    // Default Constructor to be able to create
    // arrays of ActivePlayers.
    ActivePlayer();

    // Returns Health.
    int32_t GetHealth();
    // Returns true if New Health is correct.
    bool SetHealth(int32_t health);

    // Returns Protection.
    int32_t GetProtection();
    // Returns true if New Protection is correct.
    bool SetProtection(int32_t protection);

    // Returns true if (i, j) field is visible.
    bool IsVisible(int32_t i, int32_t j);
    // Make field (i, j) visible.
    void MakeVisible(int32_t i, int32_t j);

    // Returns true if player is alive.
    bool IsAlive();

 private:
    uint64_t game_id_;

    uint64_t player_id_;
    std::string nickname_;
    std::string race_;

    int32_t health_;
    int32_t protection_;

    int32_t damage_;
    int32_t perception_;
    int32_t intellect_;
    int32_t fortune_;

    int32_t size_x_;
    int32_t size_y_;
    std::vector<std::vector<bool> > visibility_of_labyrinthmap_;

    int32_t current_x_;
    int32_t current_y_;

    int32_t previous_x_;
    int32_t previous_y_;
};

#endif  // SERVER_GAME_ACTIVEPLAYER_ACTIVEPLAYER_H_
