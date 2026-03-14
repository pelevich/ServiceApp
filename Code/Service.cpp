#include "Service.h"

/// <summary>
/// При создание объекта сразу создаем два асинхронных пайпа 
/// первый (hPipe) для работы диалогового окна между клиентом и сервисом
/// второй (hPipeCallFunction) для вызова функций
/// Создаем event для отслеживания выполнения функции
/// hEvents[0] - для отслеживания первого пайпа (hPipe)
/// hEvents[1] - для отслеживания второго пайпа (hPipeCallFunction)
/// hEvents[2] - для отслеживания функции Read
/// hEvents[3] - для отслеживания функции Write
/// </summary>
/// <param name="name">имя пайпа</param>
Service::Service(string name) {
    name = "\\\\.\\pipe\\" + name;
    hPipe = CreateNamedPipeA(
        name.c_str(),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        1024,
        1024,
        0,
        NULL
    );

    hPipeCallFunction = CreateNamedPipeA(
        "\\\\.\\pipe\\MyPipeCallFunction",
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        1024,
        1024,
        0,
        NULL
    );

    hEvents[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
    hEvents[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
    hEvents[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
    hEvents[3] = CreateEvent(NULL, TRUE, FALSE, NULL);

    overlapped[0].hEvent = hEvents[0];
    overlapped[1].hEvent = hEvents[1];
    overlapped[2].hEvent = hEvents[2];
    overlapped[3].hEvent = hEvents[3];
}

/// <summary>
/// Закрываем соединение с клиентом и отключаемся от пайпов
/// </summary>
Service::~Service() {
    DisconnectNamedPipe(hPipe);
    DisconnectNamedPipe(hPipeCallFunction);
    CloseHandle(hPipe);
    CloseHandle(hPipeCallFunction);
    for (auto i : hEvents) {
        CloseHandle(i);
    }
}

/// <summary>
/// Очищаем буффер от мусора
/// Читаем сообщение отправленое клиентом
/// Сбрасываем event
/// </summary>
/// <param name="hPipe">Дескриптор пайпа с которого читаем</param>
/// <param name="buffer">Куда будем записывать данные</param>
/// <param name="size">Размер буфера</param>
/// <param name="OVERLAPPED">Передаем ссылку на event для отслеживание выполнения функции</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Service::Read(HANDLE hPipe, char* buffer, DWORD size, OVERLAPPED* overlapped) {
    memset(buffer, 0, size);
    bool result = ReadFile(hPipe, buffer, size - 1, &bytesRead, overlapped);
    WaitForSingleObject(overlapped->hEvent, INFINITE);
    ResetEvent(overlapped->hEvent);
    return result;
}

/// <summary>
/// Отправляем сообщение клиенту
/// Сбрасываем event
/// </summary>
/// <param name="hPipe">Дескриптор пайпа с которого отправляем сообщение</param>
/// <param name="send_message">Отправляемое сообщение</param>
/// <param name="OVERLAPPED">Передаем ссылку на event для отслеживание выполнения функции</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Service::Write(HANDLE hPipe, string send_message, OVERLAPPED* overlapped) {
    bool result = WriteFile(hPipe, send_message.c_str(), send_message.size(), &bytesWritten, overlapped);
    WaitForSingleObject(overlapped->hEvent, INFINITE);
    ResetEvent(overlapped->hEvent);
    return result;
}

/// <summary>
/// Подключаем клиента
/// Сбрасываем event
/// </summary>
/// <param name="hPipe">Дескриптор пайпа к которому подключаемся</param>
/// <param name="OVERLAPPED">Передаем ссылку на event для отслеживание выполнения функции</param>
/// <returns>Возвращаем 1 в случае успеха, иначе 0</returns>
bool Service::ConnectUser(HANDLE hPipe, OVERLAPPED *overlapped) {
    bool result = ConnectNamedPipe(hPipe, overlapped);
    ResetEvent(overlapped->hEvent);
    return result;
}

/// <summary>
/// Функция для проверки нагрузки сервера
/// передоваемое число в параметре являтся и номером вызваной функции, и сколько раз будет вызвана функция
/// </summary>
/// <param name="number_repeat">Количество вызова функции</param>
void Service::RandomLoad(int number_repeat) {
    string send_message = "Function with a number " + to_string(number_repeat);
    bool result = Write(hPipeCallFunction, send_message, &overlapped[3]);
    cout << send_message << " " << result << endl;
    Sleep(1000);
    if (number_repeat - 1 != 0) {
        RandomLoad(number_repeat - 1);
    }
}