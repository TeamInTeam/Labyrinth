/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef SERVER_GAME_LABYRINTHMAP_HOSPITAL_HOSPITAL_H_
#define SERVER_GAME_LABYRINTHMAP_HOSPITAL_HOSPITAL_H_

#include <inttypes.h>
#include <string>
#include "../field/field.h"

/*
  Hospital Class.
 */
class Hospital : public Field {
 public:
    explicit Hospital(int32_t number_of_aids);

    // Returns Type of Field.
    std::string GetType();

    // Returns true if player can go
    // to this field.
    bool CanGo();

    // Returns number of Aids in Hospital.
    int32_t NumberOfAids();
    // Decrements number of Aids in Hospital.
    void DecrementNumberOfAids();

 private:
    // Type of Field.
    std::string type_;

    // Is true if player can go
    // to this field.
    bool can_go_;

    // Number Of Aids.
    int number_of_aids_;
};

#endif  // SERVER_GAME_LABYRINTHMAP_HOSPITAL_HOSPITAL_H_
