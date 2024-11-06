#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Ошибка инициализации Winsock" << endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Ошибка создания сокета" << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (InetPton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Неверный адрес/адрес не поддерживается" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Ошибка подключения" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "Подключение к серверу успешно!" << endl;

    string inputFilePath;
    string outputFilePath;
    char symbol;
    cout << "Введите путь к файлу: ";
    getline(cin, inputFilePath);
    cout << "Введите символ для добавления: ";
    cin >> symbol;
    cin.ignore();
    cout << "Введите путь для сохранения файла: ";
    getline(cin, outputFilePath);

    send(sock, inputFilePath.c_str(), inputFilePath.size(), 0);
    Sleep(100);

    send(sock, &symbol, sizeof(symbol), 0);
    Sleep(100);

    send(sock, outputFilePath.c_str(), outputFilePath.size(), 0);

    char buffer[BUFFER_SIZE] = { 0 };
    recv(sock, buffer, BUFFER_SIZE, 0);
    cout << "Ответ от сервера: " << buffer << endl;

    closesocket(sock);
    WSACleanup();

    return 0;
}