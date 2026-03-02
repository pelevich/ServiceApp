#include "Service.h"

/// <summary>
/// При создание объекта сразу создаем пайп для работы
/// </summary>
/// <param name="name">имя пайпа</param>
Service::Service(string name) {
    name = "\\\\.\\pipe\\" + name;
    hPipe = CreateNamedPipeA(
        name.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        1024,
        1024,
        0,
        NULL
    );
}

/// <summary>
/// Закрываем соединение с клиентом и отключаемся от пайпа
/// </summary>
Service::~Service() {
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

/// <summary>
/// Читаем сообщение отправленое клиентом
/// </summary>
/// <param name="hPipe">Дескриптор пайпа с которого читаем</param>
/// <param name="buffer">Куда будем записывать данные</param>
/// <param name="size">Размер буфера</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Service::Read(HANDLE hPipe, char* buffer, DWORD size) {
    bool result = ReadFile(hPipe, buffer, size - 1, &bytesRead, NULL);
    return result;
}

/// <summary>
/// Отправляем сообщение клиенту
/// </summary>
/// <param name="hPipe">Дескриптор пайпа с которого отправляем сообщение</param>
/// <param name="send_message">Отправляемое сообщение</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Service::Write(HANDLE hPipe, string send_message) {
    bool result = WriteFile(hPipe, send_message.c_str(), send_message.size(), &bytesWritten, NULL);
    return result;
}

/// <summary>
/// Подключаем клиента
/// </summary>
/// <param name="hPipe">Дескриптор пайпа к которому подключаемся</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Service::ConnectUser(HANDLE hPipe) {
    bool result = ConnectNamedPipe(hPipe, NULL);
    return result;
}