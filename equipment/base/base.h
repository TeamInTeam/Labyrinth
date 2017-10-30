/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef EQUIPMENT_BASE_BASE_H_
#define EQUIPMENT_BASE_BASE_H_

#include <inttypes.h>
#include <string>

/*
  Base class for Armor, Weapon and Aid classes.
 */
class BaseEquipment {
 public:
    // Constructor.
    BaseEquipment(std::string name,
                  std::string description,
                  int32_t wear);
    // Default constructor
    // to be able to create arrays of BaseEquipments.
    BaseEquipment();

    // Returns Name.
    virtual std::string GetName();

    // Returns Description.
    virtual std::string GetDescription();

    // Returns true if BaseEquipment is overweared.
    // Also changes Wear.
    virtual bool IncrementWear();

 protected:
    // Max Wear.
    static const int max_wear_ = 5;
    // Wear Step.
    static const int wear_step_ = 1;

    // Name.
    std::string name_;
    // Description.
    std::string description_;
    // Wear.
    int32_t wear_;
};

#endif  // EQUIPMENT_BASE_BASE_H_
