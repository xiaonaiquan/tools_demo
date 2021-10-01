#include "PipeServer.h"

CPipeServer::CPipeServer()
{

}

CPipeServer::~CPipeServer()
{

}

void CPipeServer::Initialize(const std::wstring& strReadPipe, const std::wstring& strWritePipe)
{
	m_WritePipe.Initialize(strWritePipe);
}

void CPipeServer::WriteData(const std::string& strData)
{
	m_WritePipe.WriteData(strData);
}

bool CPipeServer::IsConnected() const
{
	return m_WritePipe.IsConnected();
}