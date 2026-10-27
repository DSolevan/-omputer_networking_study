// Пример UDP-клиента (лабораторная 3)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

#define PORT 666
#define SERVERADDR "127.0.0.1"   // IP-адрес сервера

int main() {
    char buff[10 * 1024];
    cout << "UDP Demo Client\nType quit to quit\n";

    // шаг 1 - инициализация библиотеки Winsock
    WSADATA wsaData;
    if (WSAStartup(0x202, &wsaData)) {
        cout << "WSASTARTUP ERROR: " << WSAGetLastError() << "\n";
        return -1;
    }

    // шаг 2 - открытие сокета
    SOCKET my_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (my_sock == INVALID_SOCKET) {
        cout << "SOCKET() ERROR: " << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }

    // шаг 3 - обмен сообщениями с сервером
    HOSTENT *hst;
    sockaddr_in Daddr{};
    Daddr.sin_family = AF_INET;
    Daddr.sin_port = htons(PORT);

    // определение IP-адреса узла
    if (inet_addr(SERVERADDR) != INADDR_NONE)
        Daddr.sin_addr.s_addr = inet_addr(SERVERADDR);
    else if ((hst = gethostbyname(SERVERADDR)) != nullptr)
        Daddr.sin_addr.s_addr = ((unsigned long **) hst->h_addr_list)[0][0];
    else {
        cout << "Unknown Host: " << WSAGetLastError() << "\n";
        closesocket(my_sock);
        WSACleanup();
        return -1;
    }

    while (true) {
        // чтение сообщения с клавиатуры
        cout << "S<=C:";
        string SS;
        getline(cin, SS);
        if (SS == "quit") break;

        // передача сообщения на сервер
        sendto(my_sock, SS.c_str(), (int) SS.size(), 0,
               (sockaddr *) &Daddr, sizeof(Daddr));

        // приём сообщения с сервера
        sockaddr_in SRaddr{};
        int SRaddr_size = sizeof(SRaddr);
        int n = recvfrom(my_sock, buff, sizeof(buff) - 1, 0,
                         (sockaddr *) &SRaddr, &SRaddr_size);
        if (n == SOCKET_ERROR) {
            cout << "RECVFROM() ERROR: " << WSAGetLastError() << "\n";
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }
        buff[n] = '\0';

        // вывод принятого с сервера сообщения на экран
        cout << "S=>C:" << buff << "\n";
    }

    // последний шаг - выход
    closesocket(my_sock);
    WSACleanup();
    return 0;
}
