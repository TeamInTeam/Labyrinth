/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>
#include "./user_friend.h"

UserFriend::UserFriend(uint64_t id, std::string login) {
    id_ = id;
    login_ = login;
}

UserFriend::UserFriend() {
    id_ = 0;
    login_ = "";
}

uint64_t UserFriend::GetId() {
    return id_;
}

std::string UserFriend::GetLogin() {
    return login_;
}

bool UserFriend::SetLogin(std::string login) {
    // Check
    login_ = login;
    return true;
}

