#pragma once
#include <string>
#include "WritePipe.h"

class CPipeServer
{
public:
	CPipeServer();
	~CPipeServer();

	void Initialize(const std::wstring& strReadPipe, const std::wstring& strWritePipe);

	void CPipeServer::WriteData(const std::string& strData);


	bool IsConnected() const;
	
private:
	CWritePipe m_WritePipe;
};