#include <iostream>
#include <thread>
#include <chrono>
#include "User.h"
#include "Message.h"
#include "Network.h"

using namespace std;

void receiveThread(Network& net, const string& username) {
    while (true) {
        if (net.hasData()) {
            Message msg;
            if (net.receiveMessage(msg)) {
                cout << "\n💬 [" << msg.getSender() << "]: " << msg.getText() << endl;
                cout << "➜ ";
                fflush(stdout);
            }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    cout << "\n========================================" << endl;
    cout << "       🌐 СЕТЕВОЙ ЧАТ (C++/Linux) 🌐" << endl;
    cout << "========================================\n" << endl;

    cout << "Выберите режим работы:" << endl;
    cout << "  1 - Создать сервер (ждать подключения)" << endl;
    cout << "  2 - Подключиться к серверу" << endl;
    cout << "\nВаш выбор [1-2]: ";

    int choice;
    cin >> choice;

    Network net;
    if (!net.init()) {
        cerr << "❌ Ошибка инициализации сети!" << endl;
        return 1;
    }

    if (choice == 1) {
        cout << "\n📡 Запуск сервера..." << endl;
        cout << "Введите порт для прослушивания [1024-65535]: ";
        int port;
        cin >> port;

        if (port < 1024 || port > 65535) {
            cerr << "❌ Неверный порт! Должен быть в диапазоне 1024-65535" << endl;
            return 1;
        }

        cout << "\n⏳ Ожидание подключения клиента на порту " << port << "..." << endl;
        if (!net.startServer(port)) {
            cerr << "❌ Ошибка запуска сервера на порту " << port << "!" << endl;
            return 1;
        }
        cout << "✅ Клиент подключился!" << endl;

    }
    else if (choice == 2) {
        string ip;
        int port;

        cout << "\n🔌 Подключение к серверу..." << endl;
        cout << "Введите IP-адрес сервера [например, 127.0.0.1]: ";
        cin >> ip;

        cout << "Введите порт сервера: ";
        cin >> port;

        if (port < 1024 || port > 65535) {
            cerr << "❌ Неверный порт!" << endl;
            return 1;
        }

        cout << "\n⏳ Подключение к " << ip << ":" << port << "..." << endl;
        if (!net.connectToServer(ip, port)) {
            cerr << "❌ Не удалось подключиться к " << ip << ":" << port << "!" << endl;
            return 1;
        }
        cout << "✅ Подключено к серверу!" << endl;

    }
    else {
        cerr << "❌ Неверный выбор! Перезапустите программу." << endl;
        return 1;
    }

    // Ввод имени пользователя
    cout << "\n👤 Введите ваше имя пользователя: ";
    cin.ignore();
    string username;
    getline(cin, username);

    if (username.empty()) {
        username = "Аноним";
    }

    User currentUser(username, username, "");

    cout << "\n========================================" << endl;
    cout << "✅ ЧАТ ЗАПУЩЕН!" << endl;
    cout << "========================================" << endl;
    cout << "💡 Введите сообщение и нажмите Enter" << endl;
    cout << "💡 Введите 'quit' или 'exit' для выхода" << endl;
    cout << "========================================\n" << endl;

    // Запускаем поток для приёма сообщений
    thread recvThread(receiveThread, ref(net), username);
    recvThread.detach();

    // Основной цикл отправки сообщений
    string input;
    while (true) {
        cout << "✉️  [" << username << "]: ";
        getline(cin, input);

        if (input == "quit" || input == "exit" || input == "выход") {
            cout << "\n👋 Завершение работы чата..." << endl;
            break;
        }

        if (!input.empty()) {
            Message msg(input, username, "all");
            if (!net.sendMessage(msg)) {
                cerr << "❌ Ошибка отправки сообщения!" << endl;
            }
        }
    }

    net.close();
    cout << "✅ Чат завершён. До свидания!" << endl;

    return 0;
}