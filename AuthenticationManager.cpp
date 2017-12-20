#include "AuthenticationManager.h"
#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include "PrivateData.h"
#include "user.hxx"
#include "user-odb.hxx"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string/case_conv.hpp>
#include <regex>

/**
 * @author Andrey Smirnov
 */

using namespace model;
using namespace odb::core;
using namespace odb;
using std::vector;

/**
 * Length of the SHA384 block
 */
const unsigned long AuthenticationManager::SHA3_LEN = 48;

/**
 * Length of salt
 * Should be at least 32 bytes
 */
const unsigned long AuthenticationManager::SALT_LEN = 32;

// CONSTRUCTOR //
AuthenticationManager::AuthenticationManager() {
    this->db = std::tr1::shared_ptr<database>(new odb::pgsql::database(
            PrivateData::DB_USER,
            PrivateData::USER_PASSWD,
            PrivateData::DB_NAME));
}

/**
 * Gets the SHA3 hash of the password
 * @param password
 * @param salt
 * @return SHA3 hash bytes
 * @private
 */
vector<unsigned char> AuthenticationManager::get_hash(const string &password, const string &salt) { // OK
    vector<unsigned char> computed(SHA3_LEN);

    vector<unsigned char> salt_bin = to_uchar_arr(salt);
    string salt_bin_str(salt_bin.begin(), salt_bin.end());

    string tmp = password + salt_bin_str;

    const unsigned char *in = reinterpret_cast<const unsigned char *>(tmp.c_str());

    SHA384(in, tmp.length() * sizeof(unsigned char), computed.data());

    return computed;
}

/**
 * Auth method returns id of the client, if (he/she exists && password is correct)
 * Otherwise, returns 0
 * @param name unique name of the user [case insensitive]
 * @param password password for the user
 * @return 0 or id of the human
 */
unsigned long AuthenticationManager::auth(const string &name, const string &password) {
    typedef odb::query<User> Query;
    typedef odb::result<User> Result;

    transaction t(db->begin());

    std::tr1::shared_ptr<User> user(db->query_one<User>(Query::name == boost::algorithm::to_lower_copy(name)));

    if (user.get() != 0) {

        // Username exists

        if (are_equals(get_hash(password, user->get_salt()), to_uchar_arr(user->get_hash()))) {

            // Username and password are correct

            //
            // Generate keys and tokens
            //
            return user->get_id();
        }
    }

    return 0;
}

/**
 * Register user
 * @param name unique name, that does not exist [case insensitive]
 * @param password password for the user
 * @throw std::length error in case password length is less than 8 chars
 * @throw std::invalid_argument in case username contains forbidden characters
 * @return true, if registration succeeded; false otherwise
 */
bool AuthenticationManager::reg(const string &name, const string &password) {
    if (password.length() < 8)
        throw std::length_error("Password should be at least 8 chars in length!");
    if (!security_check_passed(name)) {
        throw std::invalid_argument("Username should contain only A-Z a-z 0-9 letters!");
    }
    try {
        //using namespace std;
        transaction t(db->begin());
        User user(boost::algorithm::to_lower_copy(name), password);
        db->persist(user);
        t.commit();
    } catch (...) {
        return false;
    }
    return true;
}

/**
 * Updates password of the user
 * @param name unique name of the user [case insensitive]
 * @param new_password new password of the user
 * @throw std::length error in case password length is less than 8 chars
 * @return true, if operation succeeded; false otherwise
 */
bool AuthenticationManager::update_password(const string &name, const string &new_password) {
    if (new_password.length() < 8)
        throw std::length_error("Password should be at least 8 chars in length!");

    typedef odb::query<User> Query;
    typedef odb::result<User> Result;

    transaction t(db->begin());

    std::tr1::shared_ptr<User> user(db->query_one<User>(Query::name == boost::algorithm::to_lower_copy(name)));

    if (user.get() != 0) {
        user.get()->set_password(new_password);
        db->update(*user);
        t.commit();
        return true;
    }

    return false;
}

/**
 * Checks if user exists in database
 * @param name unique name
 * @return user exists
 */
bool AuthenticationManager::user_exists(const string &name) {
    typedef odb::query<User> Query;
    typedef odb::result<User> Result;

    transaction t(db->begin());

    std::tr1::shared_ptr<User> user(db->query_one<User>(Query::name == boost::algorithm::to_lower_copy(name)));

    return user.get() != 0;
}

/**
 * Encodes bytes to hex string
 * @param data bytes
 * @return hex string
 * @private
 */
std::string AuthenticationManager::to_hex_str(const vector<unsigned char> &data) {
    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < data.size(); ++i)
        ss << std::setw(2) << std::setfill('0') << (int) data[i];
    return ss.str();
}

/**
 * Decodes hex string to bytes
 * @param hex hex string
 * @return bytes
 * @private
 */
vector<unsigned char> AuthenticationManager::to_uchar_arr(const string &hex) {

    if (hex.length() % 2 == 1) {
        throw std::invalid_argument("hex string length should be even");
    }
    vector<unsigned char> bytes(hex.length() / 2);

    for (unsigned int i = 0; i < bytes.size(); i++) {
        bytes[i] = (unsigned char) strtol(hex.substr(i * 2, 2).c_str(), NULL, 16);
    }

    return bytes;
}

/**
 * Generates salt of the stated length with secure random
 * @see SALT_LEN for salt length
 * @return salt in hex string
 */
std::string AuthenticationManager::gen_salt() {
    vector<unsigned char> tmp(SALT_LEN);
    std::uniform_int_distribution<int> uid(0, 1 << (8 * sizeof(unsigned char)));
    std::mt19937 gen{std::random_device()()};
    std::generate(tmp.begin(), tmp.end(), [&uid, &gen]() -> unsigned char { return (unsigned char) uid(gen); });
    return to_hex_str(tmp);
}

/**
 * Timing safe array comparison of byte arrays
 * @param secret secret hash from db
 * @param given client given hash
 * @attention NO RESTRICTIONS IN USER SUPPLIED LENGTH
 * @return true, if arrays are identical; false otherwise
 */
bool AuthenticationManager::are_equals(const vector<unsigned char> &secret,
                                       const vector<unsigned char> &given) {

    unsigned long nDiff = secret.size() - given.size();

    // Should ALWAYS iterate over the user-supplied length
    for (int i = 0; i < given.size() - 1; i++) {
        nDiff = nDiff | (given[i] ^ secret[i % secret.size()]);
    }

    return (nDiff == 0);
}

bool AuthenticationManager::security_check_passed(const string &input) {
    return std::regex_match(input, std::regex("^[A-Za-z0-9]+$"));
}
