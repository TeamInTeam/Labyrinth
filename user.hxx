#pragma once

#include <string>
#include <cstddef>
#include <odb/core.hxx>
#include "AuthenticationManager.h"
#include <iostream>

/**
 * @author Andrey Smirnov
 */

namespace model {
    using std::string;

/**
 * Database users
 */
#pragma db object table("users")

    class User {
    public:
        User(const string &name, const string &password)
                : name(name), is_admin(false) {

            salt = AuthenticationManager::gen_salt();
            this->hash = AuthenticationManager::to_hex_str(AuthenticationManager::get_hash(password, salt));
        }

        // inline for ORM
        string get_name() { return this->name; };

        string get_hash() { return this->hash; };

        string get_salt() { return this->salt; }

        void set_password(const string &pass) {
            this->salt = AuthenticationManager::gen_salt();
            this->hash = AuthenticationManager::to_hex_str(AuthenticationManager::get_hash(pass, salt));
        }

        unsigned long get_id() { return this->id; }

    private:
        friend class odb::access;

        User() {}  // [ORM]

#pragma db id auto  // PRIMARY_KEY
        unsigned long id; // bigint

#pragma db unique
        string name; // text
        string hash; // text
        string salt; // text

#pragma db default(0)
        unsigned short is_admin; // smallint
    };
}