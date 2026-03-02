#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

#pragma once
class Service
{
private:

public:
	HANDLE hPipe;
	DWORD bytesRead;
	DWORD bytesWritten;
	char servBuff[1024];
	string send_message = "HelloWorld";
	OVERLAPPED overlapped;

	Service(string name);
	~Service();

	bool ConnectUser(HANDLE hPipe);
	bool Read(HANDLE hPipe, char* buffer, DWORD size);
	bool Write(HANDLE hPipe, string send_message);

};

