/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef USER_FRIEND_USER_FRIEND_H_
#define USER_FRIEND_USER_FRIEND_H_

#include <inttypes.h>
#include <string>

/*
  Class for user's Friend.
 */
class UserFriend {
 public:
    // Constructor.
    UserFriend(uint64_t id, std::string login);
    // Default constructor
    // to be able to create arrays of Users.
    UserFriend();

    // Returns Id.
    uint64_t GetId();

    // Returns Login.
    std::string GetLogin();
    // Returns true if New Login is Correct.
    bool SetLogin(std::string);

 private:
    // Id.
    uint64_t id_;
    // Login.
    std::string login_;
};

#endif  // USER_FRIEND_USER_FRIEND_H_
