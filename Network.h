#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#endif

#include <string>
#include <iostream>
#include "Message.h"

class Network {
private:
    int sockfd;
    sockaddr_in addr;
    bool isServer;

public:
    Network();
    ~Network();

    // Инициализация сокета
    bool init();

    // Для сервера
    bool startServer(int port);

    // Для клиента
    bool connectToServer(const std::string& ip, int port);

    // Отправка сообщения
    bool sendMessage(const Message& msg);

    // Получение сообщения
    bool receiveMessage(Message& msg);

    // Закрытие соединения
    void close();

    // Проверка, есть ли данные для чтения
    bool hasData();
};