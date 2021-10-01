#pragma once

#include <windows.h>
#include <string>
#include <list>
#include "PipeDefine.h"

class CWritePipe
{
public:
	CWritePipe();
	~CWritePipe();

	void Initialize(const std::wstring& strPipeName);

	void WriteData(const std::string& strData);

	bool IsConnected() const;
public:
	static DWORD WINAPI WritePipeThread(PVOID p);

private:
	HANDLE m_hPipe;

	std::wstring m_strPipeName;

	// 线程ID
	DWORD	m_dwThreadId;
	HANDLE	m_hThread;

	bool    m_bConnected;

	// 写入缓冲区
	char bufferWrite[BUFFER_MAX_LEN];
};