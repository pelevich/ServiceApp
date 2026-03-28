#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;

#pragma once
class Service
{
private:

public:
	HANDLE hPipe;
	HANDLE hPipeCallFunction;
	HANDLE hEvents[4];
	OVERLAPPED overlapped[4] = { 0 };
	DWORD bytesRead;
	DWORD bytesWritten;
	char servBuff[1024];
	string send_message = "HelloWorld";

	Service(string name);
	~Service();

	bool ConnectUser(HANDLE hPipe, OVERLAPPED *overlapped);
	bool Read(HANDLE hPipe, char* buffer, DWORD size, OVERLAPPED* overlapped);
	bool Write(HANDLE hPipe, string send_message, OVERLAPPED* overlapped);
	void RandomLoad(int number_repeat);
};

