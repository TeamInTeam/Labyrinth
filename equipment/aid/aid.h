/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef EQUIPMENT_AID_AID_H_
#define EQUIPMENT_AID_AID_H_

#include <inttypes.h>
#include <string>
#include "../base/base.h"

/*
  Class for Aid.
 */
class Aid : virtual public BaseEquipment {
 public:
    // Constructor.
    Aid(std::string name,
          std::string description,
          int32_t wear,
          int32_t default_heal,
          int32_t current_heal);
    // Default constructor
    // to be able to create arrays ob Aids.
    Aid();

    // Returns Name.
    std::string GetName();

    // Returns Description.
    std::string GetDescription();

    // Returns Default Heal.
    int32_t GetDefaultHeal();

    // Returns Current Heal.
    int32_t GetCurrentHeal();
    
    // Returns true if Aid is overweared.
    // Also changes Wear and Current Heal.
    bool IncrementWear();

 private:
    // Default Heal.
    int32_t default_heal_;
    // Current Heal.
    int32_t current_heal_;
};

#endif  // EQUIPMENT_AID_AID_H_
