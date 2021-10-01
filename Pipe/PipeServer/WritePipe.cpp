#include "WritePipe.h"
#include <iostream>
#include "ChannelUtils.h"

#define WM_USER_SENDDATA WM_USER + 100

CWritePipe::CWritePipe() :m_bConnected(false)
{

}

CWritePipe::~CWritePipe()
{
	if (m_hPipe != NULL)
	{
		CloseHandle(m_hPipe);
		m_hPipe = NULL;
	}

	if (m_hThread != NULL)
	{
		::PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
		::WaitForSingleObject(m_hThread, 500);
	}
}

bool CWritePipe::IsConnected() const
{
	return m_bConnected;
}

void CWritePipe::WriteData(const std::string& strData)
{
	if (m_hThread == NULL || m_dwThreadId == 0)
		return;

	std::string* pPostData = new std::string(strData);
	::PostThreadMessage(m_dwThreadId, WM_USER_SENDDATA, (WPARAM)pPostData, 0);
}


DWORD CWritePipe::WritePipeThread(PVOID p)
{
	CWritePipe* pWritePipe = (CWritePipe*)p;
	HANDLE pipe = CreateNamedPipe(
		pWritePipe->m_strPipeName.c_str(), // name of the pipe
		PIPE_ACCESS_OUTBOUND, // 1-way pipe -- send only
		PIPE_TYPE_BYTE, // send data as a byte stream
		1, // only allow 1 instance of this pipe
		0, // no outbound buffer
		0, // no inbound buffer
		0, // use default wait time
		NULL // use default security attributes
		);
	if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
		std::wcout << "Failed to create outbound pipe instance." << std::endl;
		// look up error code here using GetLastError()
		system("pause");
		return 1;
	}
	BOOL result = ConnectNamedPipe(pipe, NULL);
	if (!result) {
		std::wcout << "Failed to make connection on named pipe." << std::endl;
		// look up error code here using GetLastError()
		CloseHandle(pipe); // close the pipe
		system("pause");
		return 1;
	}

	pWritePipe->m_bConnected = true;

	bool bRun = true;
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	while (bRun)
	{
		GetMessage(&msg, NULL, 0, 0);
		switch (msg.message)
		{
		case WM_QUIT:
		{
			bRun = false;
			break;
		}
		case WM_USER_SENDDATA:
		{
			std::string* wparam = (std::string*)msg.wParam;
			std::string data = *(wparam);

			// 组合发送包，在字符串前增加四字节，表示发送数据长度
			BYTE bytes[PACKGE_HEADER_SIZE];
			CChannelUtils::IntToBytes(data.length(), bytes);
			memcpy_s(pWritePipe->bufferWrite, PACKGE_HEADER_SIZE, bytes, PACKGE_HEADER_SIZE);
			memcpy_s(pWritePipe->bufferWrite + PACKGE_HEADER_SIZE, data.length(), data.c_str(), data.length());

			delete wparam;
			wparam = nullptr;


			DWORD numBytesWritten = 0;
			result = WriteFile(
				pipe, // handle to our outbound pipe
				pWritePipe->bufferWrite, // data to send
				PACKGE_HEADER_SIZE + data.length(), // length of data to send (bytes)
				&numBytesWritten, // will store actual amount of data sent
				NULL // not using overlapped IO
				);

			if (result) {
				std::wcout << "Number of bytes sent: " << numBytesWritten << std::endl;
			}
			else {
				std::wcout << "Failed to send data." << std::endl;
				// look up error code here using GetLastError()
			}

			break;
		}
		}
	}
	pWritePipe->m_bConnected = false;

	CloseHandle(pWritePipe->m_hPipe);
	pWritePipe->m_hPipe = NULL;

	return 0;
}

void CWritePipe::Initialize(const std::wstring& strPipeName)
{
	m_strPipeName = strPipeName;

	// 启动写入线程
	m_hThread = CreateThread(
		NULL,	// default security attributes
		0,		// use default stack size
		CWritePipe::WritePipeThread,	// thread function
		this,	// argument to thread function
		0,		// use default creation flags
		&m_dwThreadId);	// returns the thread identifier
}