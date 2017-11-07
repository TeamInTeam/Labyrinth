/* Copyright (C) 2017 Mikhail Masyagin */

#include <inttypes.h>
#include <string>

/*
  
 */
class ShowRegistration {
 public:
    // I made only default constructor,
    // because I don't want to use
    // exceptions in it.
    ShowRegistration();

    // Sets Name of New User.
    // Returns 0 if Name is correct.
    // Returns 1 if Name is incorrect.
    int32_t SetName(std::string name);
    // Returns Name of New User.
    std::string GetName();

    // Sets Surname of New User.
    // Returns 0 if Surname is correct.
    // Returns 1 if Surname is incorrect.
    int32_t SetSurname(std::string surname);
    // Returns Surname of New User.
    std::string GetSurname();

    // Sets Email of New User.
    // Returns 0 if Email is correct.
    // Returns 1 if Email is incorrect.
    int32_t SetEmail(std::string surname);
    // Returns Email of New User.
    std::string GetEmail();

    // Sets Phone Number of New User.
    // Returns 0 if Phone Number is correct.
    // Returns 1 if Phone Number is incorrect.
    int32_t SetPhoneNumber(std::string phone_number);
    // Returns Phone Number of New User.
    std::string GetPhoneNumber();

    // Sets Login of New User.
    // Returns 0 if Login is correct.
    // Returns 1 if Login is incorrect.
    int32_t SetLogin(std::string login);
    // Returns Phone Number of New User.
    std::string GetLogin();

    // Sets Password of New User.
    // Returns 0 if Password is correct.
    // Returns 1 if Password is incorrect.
    int32_t SetPassword(std::string password);
    // Returns Password of New User.
    std::string GetPassword();

    // Sends All ShowRegistration Data to Server
    // Returns 0 if Data was succesfully send.
    // Returns ...
    int32_t SendData();

 private:
    // Name and Surname.
    std::string name_;
    std::string surname_;

    // Email.
    std::string email_;

    // Phone Number.
    std::string phone_number_;

    // Login.
    std::string login_;

    // Password.
    std::string password_;
};
