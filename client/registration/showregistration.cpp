#include <inttypes.h>
#include <string>
#include "showregistration.h"

ShowRegistration::ShowRegistration() {
    // Default value for all fields is "".
    
    name_ = "";
    surname_ = "";

    email_ = "";

    phone_number_ = "";

    login_ = "";

    password_ = "";
}

int32_t ShowRegistration::SetName(std::string name) {
    // Check
    name_ = name;
    return 0;
}

std::string ShowRegistration::GetName() {
    return name_;
}

int32_t ShowRegistration::SetSurname(std::string surname) {
    // Check
    surname_ = surname;
    return 0;
}

std::string ShowRegistration::GetSurname() {
    return surname_;
}

int32_t ShowRegistration::SetEmail(std::string email) {
    // Check
    email_ = email;
    return 0;
}

std::string ShowRegistration::GetEmail() {
    return email_;
}

int32_t ShowRegistration::SetPhoneNumber(std::string phone_number) {
    // Check
    phone_number_ = phone_number;
    return 0;
}

std::string ShowRegistration::GetPhoneNumber() {
    return phone_number_;
}

int32_t ShowRegistration::SetLogin(std::string login) {
    // Check
    login_ = login;
    return 0;
}

std::string ShowRegistration::GetLogin() {
    return login_;
}

int32_t ShowRegistration::SetPassword(std::string password) {
    // Check
    password_ = password;
    return 0;
}

std::string ShowRegistration::GetPassword() {
    return password_;
}

int32_t ShowRegistration::SendData() {
    // Check
    return 0;
}
