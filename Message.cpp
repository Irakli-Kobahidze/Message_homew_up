#include "Message.h"
#include <iostream>

#ifdef _WIN32
#include <io.h>
#pragma warning(disable : 4996)
#else
#include <sys/stat.h>
#endif

Message::Message() : _text(""), _sender(""), _receiver("") {}
Message::Message(const std::string& text, const std::string& sender, const std::string& receiver)
    : _text(text), _sender(sender), _receiver(receiver) {
}

std::string Message::getText() const { return _text; }
std::string Message::getSender() const { return _sender; }
std::string Message::getReceiver() const { return _receiver; }

void Message::setText(const std::string& text) { _text = text; }
void Message::setSender(const std::string& sender) { _sender = sender; }
void Message::setReceiver(const std::string& receiver) { _receiver = receiver; }

bool Message::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть " << filename << " для записи.\n";
        return false;
    }
    file << _text << "\n" << _sender << "\n" << _receiver << "\n";
    file.close();
    setSecurePermissions(filename);
    return true;
}

bool Message::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть " << filename << " для чтения.\n";
        return false;
    }
    std::getline(file, _text);
    std::getline(file, _sender);
    std::getline(file, _receiver);
    file.close();
    return true;
}

void Message::setSecurePermissions(const std::string& filename) const {
#ifdef _WIN32
    _chmod(filename.c_str(), _S_IREAD | _S_IWRITE);
#else
    chmod(filename.c_str(), S_IRUSR | S_IWUSR);
#endif
}