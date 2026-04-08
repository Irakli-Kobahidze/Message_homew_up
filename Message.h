#pragma once
#include <string>
#include <fstream>

class Message {
private:
    std::string _text;
    std::string _sender;
    std::string _receiver;

public:
    Message();
    Message(const std::string& text, const std::string& sender, const std::string& receiver);

    std::string getText() const;
    std::string getSender() const;
    std::string getReceiver() const;

    void setText(const std::string& text);
    void setSender(const std::string& sender);
    void setReceiver(const std::string& receiver);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    void setSecurePermissions(const std::string& filename) const;
};