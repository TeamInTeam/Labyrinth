/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef USER_USER_H_
#define USER_USER_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include "../equipment/armor/armor.h"
#include "../equipment/aid/aid.h"
#include "../equipment/weapon/weapon.h"
#include "../user_friend/user_friend.h"

/*
  Class for User.
 */
class User {
 public:
    // Constructor.
    User(uint64_t id,
         std::string login,
         std::string email,
         std::string phone_number,
         std::vector<Armor> armors,
         std::vector<Aid> aids,
         std::vector<Weapon> weapons,
         int32_t money,
         std::vector<UserFriend> user_friends);
    // Default
    // to be able to create arrays of Users.
    User();

    // Returns Id.
    uint64_t GetId();

    // Returns Login.
    std::string GetLogin();
    // Returns true if New Login is Correct.
    bool SetLogin(std::string login);

    // Returns Email.
    std::string GetEmail();
    // Returns true if New Email is Correct.
    bool SetEmail(std::string email);

    // Returns Phone Number.
    std::string GetPhoneNumber();
    // Returns true if New Phone Number is Correct.
    bool SetPhoneNumber(std::string phone_number);

    // Returns Number of Armors.
    int32_t GetNumberOfArmors();
    // Returns &Armor.
    Armor& ChangeArmor(int32_t index);
    // Returns true if Armor Index'th was succesfully deleted.
    bool PopArmor(int32_t index);
    // Returns true if New Armor was succesfully added.
    bool AddArmor(Armor armor);

    // Returns Number of Aids.
    int32_t GetNumberOfAids();
    // Returns &Aid.
    Aid& ChangeAid(int32_t index);
    // Returns true if Aid Index'th was succesfully deleted.
    bool PopAid(int32_t index);
    // Returns true if New Aid was succesfully added.
    bool AddAid(Aid aid);

    // Returns Number of Weapons.
    int32_t GetNumberOfWeapons();
    // Returns &Weapon.
    Weapon& ChangeWeapon(int32_t index);
    // Returns true if Weapon Index'th was succesfully deleted.
    bool PopWeapon(int32_t index);
    // Returns true if New Weapon was succesfully added.
    bool AddWeapon(Weapon weapon);

    // Returns Number of Friends.
    int32_t GetNumberOfFriends();
    // Returns &Friend.
    UserFriend& ChangeFriend(int32_t index);
    // Returns true if Friend Index'th was succesfully deleted.
    bool PopFriend(int32_t index);
    // Returns true if New Friend was succesfully added.
    bool AddFriend(UserFriend user_friend);

 private:
    // Id.
    uint64_t id_;
    // Login, Email, Phone Number.
    std::string login_, email_, phone_number_;
    // Armors.
    std::vector<Armor> armors_;
    // Aids.
    std::vector<Aid> aids_;
    // Weapons.
    std::vector<Weapon> weapons_;
    // Money.
    int32_t money_;
    // User's Friends.
    std::vector<UserFriend> user_friends_;
};

#endif  // USER_USER_H_
