#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{

    vector <char> servBuff(1024);
    string send_message = "HelloWorld";

    WSADATA wsData; /// Создание структуры WSADATA, в которую автоматически в момент создания загружаются данные о версии сокетов, используемых ОС

    /// <summary>
    /// Первый параметр типа WORD (просто число) - запрашиваемая версия сокета ( MAKEWORD(2,2) создает число с младшим и старшим битом 2
    /// Второй параметр указатель на структуру куда будут присвоены полученые данные
    /// </summary>
    /// <returns> 0 - в случае успеха или код ошибки</returns>
    WSAStartup(MAKEWORD(2, 2), &wsData);

    /// <summary>
    /// Структура данных SOCKET, функция socket привязывает созданый сокет к параметром и возвращает данные в структуру
    /// 1 параметр: семейство адресов (IPv4 - AF_INET)
    /// 2 параметр: тип сокета (SOCK_STREAM - TCP протокол)
    /// 3 параметр: тип протокола, если создаем "сырые" сокеты (как я понял)
    /// </summary>
    /// <returns>дескриптор с номером сокета, если ошибка INVALID_SOCKET</returns>
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

    in_addr ip_to_num;

    /// <summary>
    /// Функция перевода адреса привычного для человека в структуру in_addr
    /// </summary>
    /// <returns>число меньше нуля, если ошибка</returns>
    inet_pton(AF_INET, "127.0.0.1", &ip_to_num);

    /// Структура sockaddr_in, родительская sockaddr, указываем семейство адресов и порт для дальнейшего использования в функции bind
    sockaddr_in servInfo;
    servInfo.sin_family = AF_INET;
    servInfo.sin_port = htons(5000);
    servInfo.sin_addr = ip_to_num;

    /// <summary>
    /// Привязывает сокет к конкретному адресу
    /// 1 параметр: Сокет для привязки
    /// 2 параметр: Указатель на структуру с адресом и портом
    /// 3 параметр: Размер структуры адреса
    /// </summary>
    /// <returns>0 - при успехе, SOCKET_ERROR  - при ошибке</returns>
    bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
    
    /// <summary>
    /// Переводит сокет в режим ожидания, сокет слушает порт и принимает подключение
    /// 1 параметр: слушающий сокет
    /// 2 параметр: максимальное количество подключеных процессов
    /// </summary>
    /// <returns>0 - успех, другое число ошибка</returns>
    listen(ServSock, SOMAXCONN);

    sockaddr_in clientInfo;

    int clientInfo_size = sizeof(clientInfo);
    
    /// <summary>
    /// Функция возвращает дескриптор (создает) сокета для взаимодействия с клиентом
    /// 1 параметр: слущающий сокет
    /// 2 параметр: указатель на структуру данных, куда будет записана информация о клиенте
    /// 3 параметр: указатель на размер структуры
    /// </summary>
    /// <returns></returns>
    SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

    /// <summary>
    /// Функция для приема информации с слушаемого (accept) сокета
    /// 1 параметр: сокет
    /// 2 параметр: буфер для приема информации
    /// 3 параметр: размер буфера
    /// 4 параметр: флаги
    /// </summary>
    /// <returns></returns>
    recv(ClientConn, servBuff.data(), servBuff.size(), 0);
    cout << servBuff.data();

    /// <summary>
    /// Функция для отправки информации на сокет, с которым имеется связь (recv и send одинакого рабоют как для сервера, так и для клиента)
    /// 1 параметр: сокет
    /// 2 параметр: буфер для приема информации
    /// 3 параметр: размер буфера
    /// 4 параметр: флаги
    /// </summary>
    /// <returns></returns>
    send(ClientConn, &send_message[0], send_message.size(), 0);

    /// Закрываем сокеты
    closesocket(ServSock);
    closesocket(ClientConn);
    WSACleanup();

    return 0;
}
