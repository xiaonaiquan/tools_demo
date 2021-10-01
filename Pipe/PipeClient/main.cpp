#include "PipeClient.h"
#include <iostream>

using namespace std;

wstring UTF82Unicode(const string& strUTF8)
{
	// multi char to wide char
	int length = ::MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	if (length <= 0) return L"";

	wchar_t* pwText = new wchar_t[length + 1];
	memset((void*)pwText, 0, sizeof(wchar_t) * (length + 1));
	::MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pwText, length);

	wstring strText(pwText);
	delete[] pwText;
	return strText;
}

int main()
{
	wcout.imbue(locale("chs"));

	CPipeClient client;
	client.Initialize(L"\\\\.\\pipe\\my_pipe", L"");

	auto exfunc = [](char* data,int len){
		std::string str(data, len);

		wstring temp = UTF82Unicode(str);

		std::wcout << temp.c_str() << std::endl;

	};

	client.SetReadCallback(exfunc);

	//client.WriteData("aaaaaa");

	while (1)
	{
		Sleep(1000);
	}

	system("PAUSE");
	return 0;
}