/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef EQUIPMENT_WEAPON_WEAPON_H_
#define EQUIPMENT_WEAPON_WEAPON_H_

#include <inttypes.h>
#include <string>
#include "../base/base.h"

/*
  Class for Weapon.
 */
class Weapon : virtual public BaseEquipment {
 public:
    // Constructor.
    Weapon(std::string name,
           std::string description,
           int32_t wear,
           int32_t default_damage,
           int32_t current_damage,
           int32_t max_distance);
    // Default constructor
    // to be able to create arrays ob Weapons.
    Weapon();

    // Returns Name.
    std::string GetName();

    // Returns Description.
    std::string GetDescription();

    // Returns Default Damage.
    int32_t GetDefaultDamage();

    // Returns Current Damage.
    int32_t GetCurrentDamage();

    // Returns Max Distance.
    int32_t GetMaxDistance();

    // Returns true if Weapon is overweared.
    // Also changes Wear and Current Damage.
    bool IncrementWear();

 private:
    // Default Damage.
    int32_t default_damage_;
    // Current Damage.
    int32_t current_damage_;
    // Max Distance.
    int32_t max_distance_;
};

#endif  // EQUIPMENT_WEAPON_WEAPON_H_
