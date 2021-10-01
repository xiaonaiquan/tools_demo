#include "WritePipe.h"
#include <process.h>
#include "ChannelUtils.h"

#define WM_USER_SENDDATA WM_USER + 100

CWritePipe::CWritePipe()
: m_hPipe(NULL)
, m_dwThreadId(0)
, m_hThread(NULL)
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

DWORD CWritePipe::WritePipeThread(PVOID p)
{
	CWritePipe* pWritePipe = (CWritePipe*)p;
	wstring pipiName = pWritePipe->m_strPipeName;

	// 转换管道名
	WCHAR wszClassName[256];
	memset(wszClassName, 0, sizeof(wszClassName));
	wcscpy_s(wszClassName, pipiName.c_str());

	// 连接管道
	if (!WaitNamedPipe(wszClassName, NMPWAIT_WAIT_FOREVER))
		return 1;

	// 创建文件，管道为特殊文件
	pWritePipe->m_hPipe = CreateFile(wszClassName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == pWritePipe->m_hPipe)
		return 1;

	// 设置管道状态
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL fSuccess = SetNamedPipeHandleState(
		pWritePipe->m_hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		return 1;
	}

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
								 // 从消息中读取数据
								 string* wparam = (string*)msg.wParam;
								 string data = *(wparam);

								 // 组合发送包，在字符串前增加四字节，表示发送数据长度
								 BYTE bytes[PACKGE_HEADER_SIZE];
								 CChannelUtils::IntToBytes(data.length(), bytes);
								 memcpy_s(pWritePipe->bufferWrite, PACKGE_HEADER_SIZE, bytes, PACKGE_HEADER_SIZE);
								 memcpy_s(pWritePipe->bufferWrite + PACKGE_HEADER_SIZE, data.length(), data.c_str(), data.length());

								 // 删除消息中发送的数据指针
								 delete wparam;
								 wparam = nullptr;

								 // 将数据写入管道
								 DWORD dwLen = 0;
								 if (WriteFile(pWritePipe->m_hPipe, pWritePipe->bufferWrite, PACKGE_HEADER_SIZE + data.length(), &dwLen, NULL))
								 {
									 printf("数据写入完毕共%d字节！\n", dwLen);
								 }
								 else
								 {
									 bRun = false;
									 printf("管道中断！\n", dwLen);
								 }

								 break;
		}
		}
	}

	CloseHandle(pWritePipe->m_hPipe);
	pWritePipe->m_hPipe = NULL;

	return 0;
}

void CWritePipe::Initialize(const wstring& strPipeName)
{
	this->m_strPipeName = strPipeName;

	// 启动写入线程
	m_hThread = CreateThread(
		NULL,	// default security attributes
		0,		// use default stack size
		CWritePipe::WritePipeThread,	// thread function
		this,	// argument to thread function
		0,		// use default creation flags
		&m_dwThreadId);	// returns the thread identifier
}

void CWritePipe::WriteData(const string& strData)
{
	if (m_hThread == NULL || m_dwThreadId == 0)
		return;

	string* pPostData = new string(strData);
	::PostThreadMessage(m_dwThreadId, WM_USER_SENDDATA, (WPARAM)pPostData, 0);
}