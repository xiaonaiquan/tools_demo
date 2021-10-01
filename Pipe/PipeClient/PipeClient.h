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

	/*@brief ��ʼ�� */
	void Initialize(const std::wstring& strReadPipe, const std::wstring& strWritePipe);

	/*@brief �ܵ��Ƿ����� */
	bool IsConnected() const;

	/*@brief ���ö�ȡ�ص� */
	void SetReadCallback(READ_CALLBACK funcCallback);

	/*@brief ���ùܵ���ֹ�˳��ص� */
	void SetExitCallback(EXIT_CALLBACK funcExit);

	/*@brief д������ */
	void WriteData(const std::string& strData);

private:
	CWritePipe		m_pipeWrite;	///< д��ܵ�
	CReadPipe		m_pipeRead;		///< ��ȡ�ܵ�
	bool			m_bConnected;	///< �Ƿ�����
};