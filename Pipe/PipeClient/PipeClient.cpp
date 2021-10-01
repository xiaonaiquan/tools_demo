#include "PipeClient.h"

CPipeClient::CPipeClient() :m_bConnected(false)
{

}

CPipeClient::~CPipeClient()
{

}

void CPipeClient::Initialize(const std::wstring& strReadPipe, const std::wstring& strWritePipe)
{
	//m_pipeWrite.Initialize(strWritePipe);
	m_pipeRead.Initialize(strReadPipe);
}

bool CPipeClient::IsConnected() const
{
	return m_pipeRead.IsConnected();
}

void CPipeClient::SetReadCallback(READ_CALLBACK funcCallback)
{
	m_pipeRead.SetReadCallback(funcCallback);
}

void CPipeClient::SetExitCallback(EXIT_CALLBACK funcExit)
{
	m_pipeRead.SetExitCallback(funcExit);
}

void CPipeClient::WriteData(const string& strData)
{
	m_pipeWrite.WriteData(strData);
}