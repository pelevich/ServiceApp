// CMakeServiceApp.cpp: определяет точку входа для приложения.
//

#include "ServiceApp.h"

using namespace std;

int main()
{
    Service object("MyPipe");

    object.ConnectUser(object.hPipe, &object.overlapped[0]);
    object.ConnectUser(object.hPipeCallFunction, &object.overlapped[1]);
    DWORD WaitResult = WaitForMultipleObjects(2, object.hEvents, FALSE, INFINITE);

    int index = WaitResult - WAIT_OBJECT_0;
    
    switch (index)
    {
    case 0:
        object.Read(object.hPipe, object.servBuff, sizeof(object.servBuff), &object.overlapped[2]);
        Sleep(1000);
        object.Write(object.hPipe, object.send_message, &object.overlapped[3]);

        break;

    case 1:
        object.Read(object.hPipeCallFunction, object.servBuff, sizeof(object.servBuff), &object.overlapped[2]);
        
        stringstream ss(object.servBuff);
        string token;

        ss >> token;
        
        if (token == "RandomLoad") {
            ss >> token;
            object.RandomLoad(stoi(token));
        }

        break;
    }

    return 0;
}