// CMakeServiceApp.cpp: определяет точку входа для приложения.
//

#include "ServiceApp.h"

using namespace std;

int main()
{
    Service object("MyPipe");

    bool State = object.ConnectUser(object.hPipe);

    if (!State) {
        cout << "Couldn't connect" << endl;
        return 0;
    }

    State = object.Read(object.hPipe, object.servBuff, sizeof(object.servBuff));

    if (!State) {
        cout << "Couldn't read it" << endl;
    }

    State = object.Write(object.hPipe, object.send_message);
    if (!State) {
        cout << "Couldn't send" << endl;
    }

    return 0;
}