/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_MAP_WALL_WALL_H_
#define SERVER_MAP_WALL_WALL_H_

#include <inttypes.h>
#include <string>
#include "../field/field.h"

/*
  Road Class.
 */
class Wall : public Field {
 public:
    Wall(bool is_destroyable);

    // Returns Type of Field.
    std::string GetType() const;

    // Returns true if Wall can be Destroyed
    // in this field.
    bool IsDestroyable() const;

    // Returns true if player can go
    // to this field.
    bool CanGo() const;

 private:
    // Is true if wall can be destroyed
    // in this field.
    bool is_destroyable_;
};

#endif  // SERVER_MAP_WALL_WALL_H_
