#include <iostream>
#include "PipeServer.h"

std::string Unicode2UTF8(const std::wstring& strUnicode)
{
	int length = ::WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	if (length <= 0) return "";

	char* pText = new char[length + 1];
	memset((void*)pText, 0, sizeof(char) * (length + 1));
	::WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pText, length, NULL, NULL);

	std::string strText(pText);
	delete[] pText;
	return strText;
}

int main()
{
	CPipeServer server;
	server.Initialize(L"", L"\\\\.\\pipe\\my_pipe");

	//server.WriteData("Hello Pipe World");

	while (!server.IsConnected())
	{
		Sleep(1000);
	}

	std::string zhongwen = Unicode2UTF8(L"ÖÐÎÄ");

	if (server.IsConnected())
	{
		server.WriteData(zhongwen);
	}
	while (1)
	{
		Sleep(1000);
	}
	
	system("PAUSE");
	return 0;
}