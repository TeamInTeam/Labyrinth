/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_LABYRINTHMAP_FIELD_FIELD_H_
#define SERVER_GAME_LABYRINTHMAP_FIELD_FIELD_H_

#include <inttypes.h>
#include <string>

/*
  Field Class.
 */
class Field {
 public:
    // Constructor.
    Field(std::string type, bool can_go_);
    // Destructor.
    virtual ~Field();

    // Returns Type of Field.
    virtual std::string GetType();

    // Returns true if player can go
    // to this field.
    virtual bool CanGo();

 protected:
    // Type of Field.
    std::string type_;

    // Is true if player can go
    // to this field.
    bool can_go_;
};

#endif  // SERVER_GAME_LABYRINTHMAP_FIELD_FIELD_H_
