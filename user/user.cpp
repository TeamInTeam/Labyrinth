#include <inttypes.h>
#include <string>
#include <vector>
#include "./user.h"

User::User(uint64_t id,
           std::string login,
           std::string email,
           std::string phone_number,
           std::vector<Armor> armors,
           std::vector<Aid> aids,
           std::vector<Weapon> weapons,
           int32_t money,
           std::vector<UserFriend> user_friends) {
    id_ = id;
    login_ = login;
    email_ = email;
    phone_number_ = phone_number;
    armors_ = armors;
    aids_ = aids;
    weapons_ = weapons;
    money_ = money;
    user_friends_ = user_friends;
}

User::User() {
    id_ = 0;
    login_ = email_ = phone_number_ = "";
    money_ = 0;
}

uint64_t User::GetId() {
    return id_;
}

std::string User::GetLogin() {
    return login_;
}

bool User::SetLogin(std::string login) {
    // Check
    login_ = login;
    return true;
}

std::string User::GetEmail() {
    return email_;
}

bool User::SetEmail(std::string email) {
    // Check
    email_ = email;
    return true;
}

std::string User::GetPhoneNumber() {
    return phone_number_;
}

bool User::SetPhoneNumber(std::string phone_number) {
    // Check
    phone_number_ = phone_number;
    return true;
}

int32_t User::GetNumberOfArmors() {
    return armors_.size();
}

Armor& User::ChangeArmor(int32_t index) {
    return armors_[index];
}

bool User::PopArmor(int32_t index) {
    if ((index < 0) || (index > static_cast<int>(armors_.size() - 1)))
        return false;
    armors_.erase(armors_.begin() + index);
    return true;    
}

bool User::AddArmor(Armor armor) {
    // Check
    armors_.push_back(armor);
    return true;
}

int32_t User::GetNumberOfAids() {
    return aids_.size();
}

Aid& User::ChangeAid(int32_t index) {
    return aids_[index];
}

bool User::PopAid(int32_t index) {
    if ((index < 0) || (index > static_cast<int>(aids_.size() - 1)))
        return false;
    aids_.erase(aids_.begin() + index);
    return true;    
}

bool User::AddAid(Aid aid) {
    // Check
    aids_.push_back(aid);
    return true;
}

int32_t User::GetNumberOfWeapons() {
    return weapons_.size();
}

Weapon& User::ChangeWeapon(int32_t index) {
    return weapons_[index];
}

bool User::PopWeapon(int32_t index) {
    if ((index < 0) || (index > static_cast<int>(weapons_.size() - 1)))
        return false;
    aids_.erase(aids_.begin() + index);
    return true;    
}

bool User::AddWeapon(Weapon weapon) {
    // Check
    weapons_.push_back(weapon);
    return true;
}

int32_t User::GetNumberOfFriends() {
    return user_friends_.size();
}

UserFriend& User::ChangeFriend(int32_t index) {
    return user_friends_[index];
}

bool User::PopFriend(int32_t index) {
    if ((index < 0) || (index > static_cast<int>(user_friends_.size() - 1)))
        return false;
    user_friends_.erase(user_friends_.begin() + index);
    return true;    
}

bool User::AddFriend(UserFriend user_friend) {
    // Check
    user_friends_.push_back(user_friend);
    return true;
}


