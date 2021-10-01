#pragma once
#include "ReadPipe.h"
#include "WritePipe.h"
#include "PipeDefine.h"
#include <string>

class CPipeClient
{
public:
	CPipeClient();
	~CPipeClient();

	/*@brief 初始化 */
	void Initialize(const std::wstring& strReadPipe, const std::wstring& strWritePipe);

	/*@brief 管道是否连接 */
	bool IsConnected() const;

	/*@brief 设置读取回调 */
	void SetReadCallback(READ_CALLBACK funcCallback);

	/*@brief 设置管道终止退出回调 */
	void SetExitCallback(EXIT_CALLBACK funcExit);

	/*@brief 写入数据 */
	void WriteData(const std::string& strData);

private:
	CWritePipe		m_pipeWrite;	///< 写入管道
	CReadPipe		m_pipeRead;		///< 读取管道
	bool			m_bConnected;	///< 是否连接
};