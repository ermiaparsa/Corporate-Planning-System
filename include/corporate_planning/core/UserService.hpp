#ifndef CORPORATE_PLANNING_CORE_USER_SERVICE_HPP
#define CORPORATE_PLANNING_CORE_USER_SERVICE_HPP

#include "corporate_planning/core/User.hpp"

#include <string>
#include <vector>

namespace corporate_planning::core {

struct UserRegistrationRequest {
    std::string fullName;
    std::string username;
    std::string password;
    std::string confirmPassword;
    std::string email;
    UserRole role = UserRole::VIEWER;
};

struct UserRegistrationResult {
    bool success = false;
    std::string message;
    std::string userId;
};

// Owns the in-memory user registry and all credential rules, so the GUI layer
// only has to collect input and render the outcome.
class UserService {
public:
    static constexpr std::size_t MIN_USERNAME_LENGTH = 3;
    static constexpr std::size_t MAX_USERNAME_LENGTH = 32;
    static constexpr std::size_t MIN_PASSWORD_LENGTH = 8;
    static constexpr std::size_t MAX_PASSWORD_LENGTH = 64;

    UserService();

    UserRegistrationResult registerUser(const UserRegistrationRequest& request);
    bool login(const std::string& username, const std::string& password);
    void logout();
    const User* getCurrentUser() const;

    bool usernameExists(const std::string& username) const;
    const std::vector<User>& getUsers() const;
    std::size_t getUserCount() const;

    static std::string hashPassword(const std::string& password);
    static std::string roleToDisplayName(UserRole role);

private:
    std::string validate(const UserRegistrationRequest& request) const;
    std::string generateUserId() const;

    std::vector<User> users;
    std::string currentUserId;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_USER_SERVICE_HPP
