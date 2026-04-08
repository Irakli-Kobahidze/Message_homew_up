#pragma once
#include <string>
#include <fstream>

class User {
private:
    std::string _name;
    std::string _login;
    std::string _pass;

public:
    User();
    User(const std::string& name, const std::string& login, const std::string& pass);

    std::string getName() const;
    std::string getLogin() const;
    std::string getPass() const;

    void setName(const std::string& name);
    void setLogin(const std::string& login);
    void setPass(const std::string& pass);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    void setSecurePermissions(const std::string& filename) const;
};