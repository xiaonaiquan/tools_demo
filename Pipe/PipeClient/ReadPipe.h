#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include "PipeDefine.h"

using namespace std;

class CReadPipe
{
public:
	CReadPipe();
	~CReadPipe();

	/*@brief 初始化 */
	void Initialize(const wstring& strPipeName);

	/*@brief 是否连接 */
	bool IsConnected() const;

	/*@brief 设置读取回调 */
	void SetReadCallback(READ_CALLBACK funcCallback);

	/*@brief 设置退出回调 */
	void SetExitCallback(EXIT_CALLBACK funcExit);

public:
	static void ReadPipeThread(PVOID p);

private:
	void HandleData(char* data, int len);

private:
	HANDLE			m_hPipe;
	bool			m_bConnected;

	// 管道名
	wstring			m_strReadPipe;

	// 读取回调
	READ_CALLBACK	m_funcCallback;

	// 退出回调
	EXIT_CALLBACK	m_funcExit;

	// 读取缓冲区
	char m_bufferRead[BUFFER_MAX_LEN];

	// 解包缓冲区
	char m_bufferCache[4096];

	// 缓冲区中已有数据大小
	int m_cacheDataSize;
};