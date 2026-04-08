#include "Network.h"

Network::Network() : sockfd(-1), isServer(false) {}

Network::~Network() {
    close();
}

bool Network::init() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}

bool Network::startServer(int port) {
#ifdef _WIN32
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) return false;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(sockfd);
        return false;
    }

    if (listen(sockfd, 1) == SOCKET_ERROR) {
        closesocket(sockfd);
        return false;
    }

    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    int clientSock = accept(sockfd, (sockaddr*)&clientAddr, &clientSize);

    closesocket(sockfd);
    sockfd = clientSock;
    isServer = true;
    return true;
#else
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        ::close(sockfd);
        return false;
    }

    if (listen(sockfd, 1) < 0) {
        ::close(sockfd);
        return false;
    }

    std::cout << "Сервер ожидает подключения на порту " << port << "...\n";

    sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    int clientSock = accept(sockfd, (struct sockaddr*)&clientAddr, &clientSize);

    ::close(sockfd);
    sockfd = clientSock;
    isServer = true;
    std::cout << "Клиент подключился!\n";
    return true;
#endif
}

bool Network::connectToServer(const std::string& ip, int port) {
#ifdef _WIN32
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) return false;

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    addr.sin_port = htons(port);

    if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(sockfd);
        return false;
    }

    isServer = false;
    return true;
#else
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    addr.sin_port = htons(port);

    if (::connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        ::close(sockfd);
        return false;
    }

    isServer = false;
    std::cout << "Подключено к серверу!\n";
    return true;
#endif
}

bool Network::sendMessage(const Message& msg) {
    if (sockfd < 0) return false;

    std::string data = msg.getText() + "|" + msg.getSender() + "|" + msg.getReceiver();

#ifdef _WIN32
    int len = data.length();
    send(sockfd, (const char*)&len, sizeof(len), 0);
    return send(sockfd, data.c_str(), data.length(), 0) != SOCKET_ERROR;
#else
    int len = data.length();
    send(sockfd, &len, sizeof(len), 0);
    return send(sockfd, data.c_str(), data.length(), 0) != -1;
#endif
}

bool Network::receiveMessage(Message& msg) {
    if (sockfd < 0) return false;

    int len = 0;

#ifdef _WIN32
    if (recv(sockfd, (char*)&len, sizeof(len), 0) <= 0) return false;

    char* buffer = new char[len + 1];
    int received = recv(sockfd, buffer, len, 0);
    buffer[len] = '\0';
#else
    if (::recv(sockfd, &len, sizeof(len), 0) <= 0) return false;

    char* buffer = new char[len + 1];
    int received = ::recv(sockfd, buffer, len, 0);
    buffer[len] = '\0';
#endif

    std::string data(buffer);
    delete[] buffer;

    // Разбираем строку
    size_t pos1 = data.find('|');
    size_t pos2 = data.find('|', pos1 + 1);

    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        msg.setText(data.substr(0, pos1));
        msg.setSender(data.substr(pos1 + 1, pos2 - pos1 - 1));
        msg.setReceiver(data.substr(pos2 + 1));
        return true;
    }

    return false;
}

void Network::close() {
    if (sockfd >= 0) {
#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#else
        ::close(sockfd);
#endif
    }
    sockfd = -1;
}

bool Network::hasData() {
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sockfd, FIONREAD, &mode);
    return mode > 0;
#else
    int bytes_available = 0;
    ioctl(sockfd, FIONREAD, &bytes_available);
    return bytes_available > 0;
#endif
}