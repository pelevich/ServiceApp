#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsData;

    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

    in_addr ip_to_num;
    erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);

    sockaddr_in servInfo;

    servInfo.sin_family = AF_INET;
    servInfo.sin_port = htons(5000);
    servInfo.sin_addr = ip_to_num;

    erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
    
    erStat = listen(ServSock, SOMAXCONN);

    sockaddr_in clientInfo;

    int clientInfo_size = sizeof(clientInfo);

    SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

    vector <char> servBuff(1024), clientBuff(1024);
    short packet_size = 0;

    while (true) {
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
        cout << servBuff.data();
    }

    return 0;
}
