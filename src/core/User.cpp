#include "corporate_planning/core/User.hpp"
#include <iostream>

using namespace std;

namespace corporate_planning::core {

User::User()
    : userId(""),
      fullName(""),
      username(""),
      passwordHash(""),
      email(""),
      role(UserRole::VIEWER),
      isActive(true),
      lastLogin("Never") {
}
User::User(const string& id, const string& name, const string& uname,
           const string& pwdHash, const string& mail, UserRole userRole)
    : userId(id),
      fullName(name),
      username(uname),
      passwordHash(pwdHash),
      email(mail),
      role(userRole),
      isActive(true),
      lastLogin("Never") {
}

// Getters
string User::getUserId() const {
    return userId;
}

string User::getFullName() const {
    return fullName;
}

string User::getUsername() const {
    return username;
}

string User::getPasswordHash() const {
    return passwordHash;
}

string User::getEmail() const {
    return email;
}

UserRole User::getRole() const {
    return role;
}

string User::getRoleString() const {
    switch (role) {
        case UserRole::ADMIN:
            return "ADMIN";
        case UserRole::PLANNER:
            return "PLANNER";
        case UserRole::ANALYST:
            return "ANALYST";
        case UserRole::VIEWER:
            return "VIEWER";
        default:
            return "UNKNOWN";
    }
}

bool User::getIsActive() const {
    return isActive;
}

string User::getLastLogin() const {
    return lastLogin;
}

// Setters
void User::setUserId(const string& id) {
    userId = id;
}

void User::setFullName(const string& name) {
    fullName = name;
}

void User::setUsername(const string& uname) {
    username = uname;
}

void User::setPasswordHash(const string& pwdHash) {
    passwordHash = pwdHash;
}

void User::setEmail(const string& mail) {
    email = mail;
}

void User::setRole(UserRole userRole) {
    role = userRole;
}

void User::setIsActive(bool active) {
    isActive = active;
}

void User::setLastLogin(const string& loginTime) {
    lastLogin = loginTime;
}

// Business Logic
bool User::authenticate(const string& enteredUsername, const string& enteredPasswordHash) const {
    if (!isActive) {
        return false;
    }
    return (username == enteredUsername && passwordHash == enteredPasswordHash);
}

bool User::hasAdminPrivileges() const {
    return (isActive && role == UserRole::ADMIN);
}

} // namespace corporate_planning::core
