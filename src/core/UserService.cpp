#include "corporate_planning/core/UserService.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <sstream>

using namespace std;

namespace corporate_planning::core {

namespace {

const string PASSWORD_SALT = "corporate-planning-v1";

string trim(const string& value) {
    const auto first = value.find_first_not_of(" \t\n\r");
    if (first == string::npos) {
        return "";
    }
    const auto last = value.find_last_not_of(" \t\n\r");
    return value.substr(first, last - first + 1);
}

string toLower(const string& value) {
    string lowered = value;
    transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char character) {
        return static_cast<char>(tolower(character));
    });
    return lowered;
}

bool isValidUsername(const string& username) {
    return all_of(username.begin(), username.end(), [](unsigned char character) {
        return isalnum(character) || character == '_' || character == '.';
    });
}

bool hasCharacterClass(const string& password) {
    const bool hasLetter = any_of(password.begin(), password.end(), [](unsigned char character) {
        return isalpha(character) != 0;
    });
    const bool hasDigit = any_of(password.begin(), password.end(), [](unsigned char character) {
        return isdigit(character) != 0;
    });
    return hasLetter && hasDigit;
}

} // namespace

UserService::UserService()
    : users() {
}

UserRegistrationResult UserService::registerUser(const UserRegistrationRequest& request) {
    UserRegistrationResult result;

    UserRegistrationRequest normalized = request;
    normalized.fullName = trim(request.fullName);
    normalized.username = trim(request.username);
    normalized.email = trim(request.email);

    const string validationError = validate(normalized);
    if (!validationError.empty()) {
        result.success = false;
        result.message = validationError;
        return result;
    }

    User user(
        generateUserId(),
        normalized.fullName.empty() ? normalized.username : normalized.fullName,
        normalized.username,
        hashPassword(normalized.password),
        normalized.email,
        normalized.role
    );
    users.push_back(user);

    result.success = true;
    result.userId = user.getUserId();
    result.message = "Account \"" + user.getUsername() + "\" created successfully as "
                     + roleToDisplayName(user.getRole()) + ".";
    return result;
}

bool UserService::usernameExists(const string& username) const {
    const string target = toLower(trim(username));
    return any_of(users.begin(), users.end(), [&target](const User& user) {
        return toLower(user.getUsername()) == target;
    });
}

bool UserService::login(const string& username, const string& password) {
    const string target = toLower(trim(username));
    for (const auto& user : users) {
        if (toLower(user.getUsername()) == target
            && user.authenticate(user.getUsername(), hashPassword(password))) {
            currentUserId = user.getUserId();
            return true;
        }
    }
    return false;
}

void UserService::logout() {
    currentUserId.clear();
}

const User* UserService::getCurrentUser() const {
    for (const auto& user : users) {
        if (user.getUserId() == currentUserId && user.getIsActive()) {
            return &user;
        }
    }
    return nullptr;
}

const vector<User>& UserService::getUsers() const {
    return users;
}

size_t UserService::getUserCount() const {
    return users.size();
}

string UserService::hashPassword(const string& password) {
    const size_t digest = hash<string>{}(PASSWORD_SALT + password);
    ostringstream stream;
    stream << hex << setw(16) << setfill('0') << digest;
    return stream.str();
}

string UserService::roleToDisplayName(UserRole role) {
    switch (role) {
        case UserRole::ADMIN:
            return "Administrator";
        case UserRole::PLANNER:
            return "Planning Specialist";
        case UserRole::ANALYST:
            return "Financial Analyst";
        case UserRole::VIEWER:
            return "Auditor / Viewer";
        default:
            return "Unknown";
    }
}

string UserService::validate(const UserRegistrationRequest& request) const {
    if (request.username.empty()) {
        return "Username is required.";
    }
    if (request.username.size() < MIN_USERNAME_LENGTH || request.username.size() > MAX_USERNAME_LENGTH) {
        return "Username must be between " + to_string(MIN_USERNAME_LENGTH) + " and "
               + to_string(MAX_USERNAME_LENGTH) + " characters.";
    }
    if (!isValidUsername(request.username)) {
        return "Username may only contain letters, digits, underscore and dot.";
    }
    if (usernameExists(request.username)) {
        return "Username \"" + request.username + "\" is already registered.";
    }
    if (request.password.empty()) {
        return "Password is required.";
    }
    if (request.password.size() < MIN_PASSWORD_LENGTH || request.password.size() > MAX_PASSWORD_LENGTH) {
        return "Password must be between " + to_string(MIN_PASSWORD_LENGTH) + " and "
               + to_string(MAX_PASSWORD_LENGTH) + " characters.";
    }
    if (!hasCharacterClass(request.password)) {
        return "Password must contain at least one letter and one digit.";
    }
    if (request.confirmPassword.empty()) {
        return "Please confirm the password.";
    }
    if (request.password != request.confirmPassword) {
        return "Password and confirmation do not match.";
    }
    if (!request.email.empty()) {
        const auto atPosition = request.email.find('@');
        const auto dotPosition = request.email.rfind('.');
        if (atPosition == string::npos || dotPosition == string::npos
            || atPosition == 0 || dotPosition < atPosition + 2
            || dotPosition + 1 >= request.email.size()) {
            return "Email address format is invalid.";
        }
    }
    return "";
}

string UserService::generateUserId() const {
    ostringstream stream;
    stream << "USR-" << setw(4) << setfill('0') << (users.size() + 1);
    return stream.str();
}

} // namespace corporate_planning::core
