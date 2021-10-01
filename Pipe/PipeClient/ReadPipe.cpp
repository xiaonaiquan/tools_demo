#include "ReadPipe.h"
#include <process.h>
#include "ChannelUtils.h"

CReadPipe::CReadPipe()
	: m_cacheDataSize(0)
	, m_funcCallback(nullptr)
	, m_funcExit(nullptr)
	, m_hPipe(NULL)
	, m_bConnected(false)
{
}

CReadPipe::~CReadPipe()
{
	if (m_hPipe != NULL)
	{
		CloseHandle(m_hPipe);
		m_hPipe = NULL;
	}
}

void CReadPipe::ReadPipeThread(PVOID p)
{
	CReadPipe* pReadPipe = (CReadPipe*)p;
	wstring pipiName = pReadPipe->m_strReadPipe;

	// 转换管道名
	WCHAR wszClassName[256];
	memset(wszClassName, 0, sizeof(wszClassName));
	wcscpy_s(wszClassName, pipiName.c_str());

	// 连接管道 这里是说管道存在，等待链接，如果管道不存在，失败返回0
	if (!WaitNamedPipe(wszClassName, NMPWAIT_WAIT_FOREVER))
	{
		printf("Error! 连接服务端发送线程失败！\n");
		return;
	}

	// 创建文件
	pReadPipe->m_hPipe = CreateFile(wszClassName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == pReadPipe->m_hPipe)
		return;

	// 设置管道连接状态
	pReadPipe->m_bConnected = true;

	// 遍历从管道中读取数据
	DWORD dwLenRead = 0;
	while (true)
	{
		// 重置读缓冲区
		memset(pReadPipe->m_bufferRead, 0, BUFFER_MAX_LEN);

		// 接收服务端发回的数据，该函数阻塞当前线程
		BOOL bSuccess = ReadFile(pReadPipe->m_hPipe, pReadPipe->m_bufferRead, BUFFER_MAX_LEN, &dwLenRead, NULL); // 读取管道中的内容（管道是一种特殊的文件）

		// 若接收数据为空，管道被关闭
		if (!bSuccess || dwLenRead == 0)
			break;

		// 处理数据
		pReadPipe->HandleData(pReadPipe->m_bufferRead, dwLenRead);
	}

	// 关闭管道句柄
	CloseHandle(pReadPipe->m_hPipe);
	// 管道设置为NULL
	pReadPipe->m_hPipe = NULL;

	// 设置管道连接状态
	pReadPipe->m_bConnected = false;

	// 退出
	if (pReadPipe->m_funcExit != nullptr)
		pReadPipe->m_funcExit();
}

// 处理粘包
void CReadPipe::HandleData(char* data, int len)
{
	// 将数据存入缓冲区
	memcpy_s(m_bufferCache + m_cacheDataSize, len, data, len);

	// 缓冲区已有数据大小
	m_cacheDataSize += len;

	while (true)
	{
		// 如果包数据大小小于4，不能获取包长度，停止
		if (m_cacheDataSize < PACKGE_HEADER_SIZE)
			break;

		// 求取数据包大小
		BYTE* ary = (BYTE*)m_bufferCache;
		int dataSize = CChannelUtils::BytesToInt(ary, 0);

		// 判断数据包大小是否够一包的大小
		if (m_cacheDataSize < PACKGE_HEADER_SIZE + dataSize)
			break;

		// 获取一包数据
		char onePackage[4096];
		memset(onePackage, 0, 4096);
		memcpy_s(onePackage, dataSize, m_bufferCache + PACKGE_HEADER_SIZE, dataSize);

		// 处理数据
		if (m_funcCallback)
		{
			m_funcCallback(onePackage, dataSize);
		}

		// 从缓冲区中移除已处理包数据
		m_cacheDataSize = m_cacheDataSize - (PACKGE_HEADER_SIZE + dataSize);
		char remainingData[4096];
		memset(remainingData, 0, 4096);
		memcpy_s(remainingData, m_cacheDataSize, m_bufferCache + PACKGE_HEADER_SIZE + dataSize, m_cacheDataSize);

		memset(m_bufferCache, 0, 4096);
		memcpy_s(m_bufferCache, m_cacheDataSize, remainingData, m_cacheDataSize);
	}
}

void CReadPipe::Initialize(const wstring& strPipeName)
{
	this->m_strReadPipe = strPipeName;

	// 启动线程
	_beginthread(CReadPipe::ReadPipeThread, NULL, this);
}

bool CReadPipe::IsConnected() const
{
	return m_bConnected;
}

void CReadPipe::SetReadCallback(READ_CALLBACK funcCallback)
{
	this->m_funcCallback = funcCallback;
}

void CReadPipe::SetExitCallback(EXIT_CALLBACK funcExit)
{
	this->m_funcExit = funcExit;
}