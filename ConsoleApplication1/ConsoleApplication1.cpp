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
        cerr << "������ ������������� Winsock" << endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "������ �������� ������" << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (InetPton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "�������� �����/����� �� ��������������" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "������ �����������" << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "����������� � ������� �������!" << endl;

    string inputFilePath;
    string outputFilePath;
    char symbol;
    cout << "������� ���� � �����: ";
    getline(cin, inputFilePath);
    cout << "������� ������ ��� ����������: ";
    cin >> symbol;
    cin.ignore();
    cout << "������� ���� ��� ���������� �����: ";
    getline(cin, outputFilePath);

    send(sock, inputFilePath.c_str(), inputFilePath.size(), 0);
    Sleep(100);

    send(sock, &symbol, sizeof(symbol), 0);
    Sleep(100);

    send(sock, outputFilePath.c_str(), outputFilePath.size(), 0);

    char buffer[BUFFER_SIZE] = { 0 };
    recv(sock, buffer, BUFFER_SIZE, 0);
    cout << "����� �� �������: " << buffer << endl;

    closesocket(sock);
    WSACleanup();

    return 0;
}