#pragma once
#include <string>
#include <mutex>
#include <atomic>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "curl/curl.h"

class CurlDownDelegate;

class CurlDown
{
public:
	CurlDown();
	CurlDown(std::string downUrl, std::string filePath);
	~CurlDown();

public:
	std::string m_DownloadUrl; // ����URL
	std::string m_FilePath; // ���ش洢��ַ

	double mFileLenth; // �����ļ���С
	long long mHasDownLenth;

	long timeout; // ����ʱʱ�� Ϊ�˲����� ���ó�ʱʱ��Ϊ2�� ����Ƿ���ʽ�汾 ��Ϊ20�볬ʱʱ��
	bool Download(); // ���ط���
	double getDownloadFileLenth(); // �����ļ���С����
	void downloadControler(); // ���ؿ��Ʒ���
	long long getLocalFileLength(); // ��ǰ�����ļ���С

	void setStopDown();// ֹͣ����
	void setDelegate(const std::shared_ptr<CurlDownDelegate>& delegate);
	//CurlDownDelegate *m_pDelegate;
	std::shared_ptr<CurlDownDelegate> m_pDelegate;

	enum ErrorCode
	{
		// Error caused by creating a file to store downloaded data
		kCreateFile,
		/** Error caused by network
		-- network unavaivable
		-- timeout
		-- ...
		*/
		kNetwork,
		/** There is not a new version
		*/
		kNoNewVersion,
		/** Error caused in uncompressing stage
		-- can not open zip file
		-- can not read file global information
		-- can not read file information
		-- can not create a directory
		-- ...
		*/
		kUncompress,
	};

	double dStartTime;
	double dLastTime;
	double dLastSize;
private:
	std::string m_FileName; // �����ļ�����
	std::atomic<bool> m_bStop;
};


class CurlDownDelegate
{
public:
	/* @brief Call back function for error
	@param errorCode Type of error
	*/
	virtual void onError(CurlDown::ErrorCode errorCode) {};
	/** @brief Call back function for recording downloading percent
	@param percent How much percent downloaded
	@warn This call back function just for recording downloading percent.
	AssetsManager will do some other thing after downloading, you should
	write code in onSuccess() after downloading.
	*/
	virtual void onProgress(double percent, void *delegate, std::string filefullPath) {};
	/** @brief Call back function for success
	*/
	virtual void onSuccess(std::string filefullPath) {};
};
