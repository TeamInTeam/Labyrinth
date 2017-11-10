/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_LABYRINTHMAP_ARMOURY_ARMOURY_H_
#define SERVER_GAME_LABYRINTHMAP_ARMOURY_ARMOURY_H_

#include <inttypes.h>
#include <string>
#include "../field/field.h"

/*
  Armoury Class.
 */
class Armoury : public Field {
 public:
    explicit Armoury(int32_t number_of_weapons);

    // Returns Type of Field.
    std::string GetType() const;

    // Returns true if player can go
    // to this field.
    bool CanGo() const;

    // Returns number of Weapons in Armoury.
    int32_t NumberOfWeapons() const;
    // Decrements number of Weapons in Armoury.
    void DecrementNumberOfWeapons();

 private:
    // Type of Field.
    std::string type_;

    // Is true if player can go
    // to this field.
    bool can_go_;

    // Number Of Aids.
    int number_of_weapons_;
};

#endif  // SERVER_GAME_LABYRINTHMAP_ARMOURY_ARMOURY_H_
