#include "User.h"
#include <iostream>

// Адаптация под Windows и Linux
#ifdef _WIN32
#include <io.h>
#pragma warning(disable : 4996) // Отключаем предупреждения о deprecated в VS
#else
#include <sys/stat.h>
#endif

User::User() : _name(""), _login(""), _pass("") {}
User::User(const std::string& name, const std::string& login, const std::string& pass)
    : _name(name), _login(login), _pass(pass) {
}

std::string User::getName() const { return _name; }
std::string User::getLogin() const { return _login; }
std::string User::getPass() const { return _pass; }

void User::setName(const std::string& name) { _name = name; }
void User::setLogin(const std::string& login) { _login = login; }
void User::setPass(const std::string& pass) { _pass = pass; }

bool User::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть " << filename << " для записи.\n";
        return false;
    }
    file << _name << "\n" << _login << "\n" << _pass << "\n";
    file.close();
    setSecurePermissions(filename);
    return true;
}

bool User::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть " << filename << " для чтения.\n";
        return false;
    }
    std::getline(file, _name);
    std::getline(file, _login);
    std::getline(file, _pass);
    file.close();
    return true;
}

void User::setSecurePermissions(const std::string& filename) const {
#ifdef _WIN32
    _chmod(filename.c_str(), _S_IREAD | _S_IWRITE); // Windows: чтение/запись только владельцу
#else
    chmod(filename.c_str(), S_IRUSR | S_IWUSR);     // Linux: 0600
#endif
}