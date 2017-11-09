/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_LABYRINTHMAP_MINE_MINE_H_
#define SERVER_GAME_LABYRINTHMAP_MINE_MINE_H_

#include <inttypes.h>
#include <string>
#include "../field/field.h"

/*
  Mine Class.
 */
class Mine : public Field {
 public:
    Mine(int32_t damage, int32_t visibility);

    // Returns Type of Field.
    std::string GetType();

    // Returns true if player can go
    // to this field.
    bool CanGo();

    // Returns Damage of Mine.
    int32_t GetDamage();
    // Returns Visibility of Mine.
    int32_t GetVisibility();

 private:
    // Type of Field.
    std::string type_;

    // Is true if player can go
    // to this field.
    bool can_go_;

    // Mine Damage.
    int32_t damage_;
    // Mine Visibility.
    int32_t visibility_;
};

#endif  // SERVER_GAME_LABYRINTHMAP_MINE_MINE_H_
