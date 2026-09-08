#ifndef CORPORATE_PLANNING_CORE_USER_HPP
#define CORPORATE_PLANNING_CORE_USER_HPP

#include <string>

namespace corporate_planning::core {

enum class UserRole {
    ADMIN,       // مدیر کل سیستم
    PLANNER,     // کارشناس برنامه‌ریزی تولید و بنگاه
    ANALYST,     // تحلیل‌گر مالی و مدل‌ساز هوش مصنوعی
    VIEWER       // مشاهده‌کننده گزارش‌ها
};

class User {
public:
    User();
    User(const std::string& id, const std::string& name, const std::string& uname,
         const std::string& pwdHash, const std::string& mail, UserRole userRole);

    // Getters
    std::string getUserId() const;
    std::string getFullName() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    std::string getEmail() const;
    UserRole getRole() const;
    std::string getRoleString() const;
    bool getIsActive() const;
    std::string getLastLogin() const;

    // Setters
    void setUserId(const std::string& id);
    void setFullName(const std::string& name);
    void setUsername(const std::string& uname);
    void setPasswordHash(const std::string& pwdHash);
    void setEmail(const std::string& mail);
    void setRole(UserRole userRole);
    void setIsActive(bool active);
    void setLastLogin(const std::string& loginTime);

    // Business Logic
    bool authenticate(const std::string& enteredUsername, const std::string& enteredPasswordHash) const;
    bool hasAdminPrivileges() const;

private:
    std::string userId;
    std::string fullName;
    std::string username;
    std::string passwordHash;
    std::string email;
    UserRole role;
    bool isActive;
    std::string lastLogin;
};

} // namespace corporate_planning::core

#endif // CORPORATE_PLANNING_CORE_USER_HPP
