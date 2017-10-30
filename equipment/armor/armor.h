/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef EQUIPMENT_ARMOR_ARMOR_H_
#define EQUIPMENT_ARMOR_ARMOR_H_

#include <inttypes.h>
#include <string>
#include "../base/base.h"

/*
  Class for Armor.
 */
class Armor : virtual public BaseEquipment {
 public:
    // Constructor.
    Armor(std::string name,
          std::string description,
          int32_t wear,
          int32_t default_protection,
          int32_t current_protection);
    // Default constructor
    // to be able to create arrays ob Armors.
    Armor();

    // Returns Name.
    std::string GetName();

    // Returns Description.
    std::string GetDescription();

    // Returns Default Protection.
    int32_t GetDefaultProtection();

    // Returns Current Protection.
    int32_t GetCurrentProtection();

    // Returns true if Armor is overweared.
    // Also changes Wear and Current Protection.
    bool IncrementWear();

 private:
    // Default Protection.
    int32_t default_protection_;
    // Current Protection.
    int32_t current_protection_;
};

#endif  // EQUIPMENT_ARMOR_ARMOR_H_
