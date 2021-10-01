#pragma once

#include <windows.h>
#include <string>
#include <list>
#include "PipeDefine.h"

using namespace std;


class CWritePipe
{
public:
	CWritePipe();
	~CWritePipe();

	/*@brief 初始化管道写入线程 */
	void Initialize(const wstring& strPipeName);

	/*@brief 写入数据 */
	void WriteData(const string& strData);

public:
	static DWORD WINAPI WritePipeThread(PVOID p);

private:
	HANDLE	m_hPipe;

	// 管道名
	wstring	m_strPipeName;

	// 写入缓冲区
	char bufferWrite[BUFFER_MAX_LEN];

	// 线程ID
	DWORD	m_dwThreadId;
	HANDLE	m_hThread;
};