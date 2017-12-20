#pragma once

#include <string>
#include <boost/tr1/memory.hpp>
#include <odb/core.hxx>
#include <vector>

/**
 * @author Andrey Smirnov
 */

namespace model {
    using std::string;

    class AuthenticationManager {
    public:
        AuthenticationManager();

        /**
         * Authenticate user
         * @param name unique name [case insensitive]
         * @param password password of length >= 8 characters
         * @throw std::length error in case password length is less than 8 chars
         * @return unique id for the client, if the authentication succeeded; otherwise, returns 0
         */
        unsigned long auth(const string &name, const string &password);

        /**
         * Updates the password for the given user
         * @param name unique name [case insensitive]
         * @param new_password password of length >= 8 characters
         * @throw std::length error in case password length is less than 8 chars
         * @return true, if operation succeeded; false otherwise
         */
        bool update_password(const string &name, const string &new_password);

        /**
         * Register the user with the given password
         * @param name unique name [case insensitive]
         * @param new_password password of length >= 8 characters
         * @throw std::length error in case password length is less than 8 chars
         * @throw std::invalid_argument in case username contains forbidden characters
         * @return true, if operation succeeded; false otherwise
         */
        bool reg(const string &name, const string &password);


        /**
         * Checks if user exists in database
         * @param name unique name [case insensitive]
         * @return true, if user exists; otherwise - false
         */
        bool user_exists(const string &name);

    private:
        friend class User;

        std::tr1::shared_ptr <odb::core::database> db;

        static const unsigned long SHA3_LEN;
        static const unsigned long SALT_LEN;


        static std::vector<unsigned char> get_hash(const string &password, const string &salt);

        static bool are_equals(const std::vector<unsigned char> &secret,
                               const std::vector<unsigned char> &given);

        static std::string gen_salt();

        static bool security_check_passed(const string &input);

        static std::vector<unsigned char> to_uchar_arr(const string &string);

        static string to_hex_str(const std::vector<unsigned char> &data);

    };
}