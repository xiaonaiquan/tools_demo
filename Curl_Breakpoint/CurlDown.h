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
	std::string m_DownloadUrl; // 下载URL
	std::string m_FilePath; // 本地存储地址

	double mFileLenth; // 下载文件大小
	long long mHasDownLenth;

	long timeout; // 请求超时时间 为了测试用 设置超时时间为2秒 如果是发正式版本 改为20秒超时时间
	bool Download(); // 下载方法
	double getDownloadFileLenth(); // 下载文件大小方法
	void downloadControler(); // 下载控制方法
	long long getLocalFileLength(); // 当前下载文件大小

	void setStopDown();// 停止下载
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
	std::string m_FileName; // 下载文件名称
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
