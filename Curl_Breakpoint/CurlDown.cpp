#include "CurlDown.h"
#include <sys/timeb.h>

std::wstring ASCII2Unicode(const std::string& strASCII)
{
	int length = MultiByteToWideChar(CP_ACP, 0, strASCII.c_str(), -1, NULL, 0);
	if (length <= 0) return L"";

	wchar_t* pText = new wchar_t[length + 1];
	memset((void*)pText, 0, sizeof(wchar_t) * (length + 1));
	::MultiByteToWideChar(CP_ACP, 0, strASCII.c_str(), -1, pText, length);

	std::wstring strText(pText);
	delete[] pText;
	return strText;
}

CurlDown::CurlDown() : m_FileName("")
{
	m_bStop = false;
}


CurlDown::CurlDown(std::string downUrl, std::string filePath) : m_DownloadUrl(downUrl), m_FilePath(filePath), m_pDelegate(nullptr), dStartTime(0), dLastTime(0), dLastSize(0),
mHasDownLenth(0)
{

}

CurlDown::~CurlDown()
{

}

void CurlDown::setDelegate(const std::shared_ptr<CurlDownDelegate>& delegate)
{
	m_pDelegate = delegate;
}

long long CurlDown::getLocalFileLength()
{
	FILE *fp = fopen(m_FilePath.c_str(), "r");
	if (fp == NULL)
	{
		std::cout << "can't open file" << std::endl;
	}
	_fseeki64(fp, 0, SEEK_END);
	long long length = _ftelli64(fp);
	fclose(fp);
	return length;
}


/* 得到远程文件的大小, 要下载的文件大小 */ // 参考的那个不对 获取不到大小
double CurlDown::getDownloadFileLenth()
{
	//	double filesize = 0.0;
	CURL *handle = curl_easy_init();
	//    http://curl.haxx.se/libcurl/c/ftpgetinfo.html
	//    curl_easy_setopt(curl, CURLOPT_URL, ftpurl);
	//    /* No download if the file */
	//    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
	//    /* Ask for filetime */
	//    curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
	//    /* No header output: TODO 14.1 http-style HEAD output for ftp */
	//    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
	//    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
	//    /* Switch on full protocol/debug output */
	//    /* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */

	curl_easy_setopt(handle, CURLOPT_URL, m_DownloadUrl.c_str());
	/* No download if the file */
	curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);
	/* Ask for filetime */
	curl_easy_setopt(handle, CURLOPT_HEADER, 0L); // 0 不打印日志 1打印日志

	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);

	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);

	if (curl_easy_perform(handle) == CURLE_OK) {
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &mFileLenth);
		printf("filesize : %0.0f bytes\n", mFileLenth);
	}
	else {
		mFileLenth = -1;
	}
	return mFileLenth;
}

bool IsFileExist(LPCWSTR FileDirectory)
{
	bool bret = true;
	HANDLE hFile = CreateFile(FileDirectory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		bret = false;
	CloseHandle(hFile);
	return bret;
}

static size_t my_write_func(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *fp = (FILE*)userdata;
	size_t written = fwrite(ptr, size, nmemb, fp);
	return written;
}

static int my_progress_func(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	//    double curpercent = nowDownloaded / totalToDownload *100;//2001619
	//    if (totalToDownload == 0) {
	//        return 0;
	//    }
	// 下载总值 上面注释的如果进入续传就不准确了
	CurlDown* curlDown = (CurlDown*)ptr;
	if (!curlDown || curlDown->mFileLenth == 0 || nowDownloaded == 0) return 0;
	double nowDown = (curlDown->mFileLenth - totalToDownload + nowDownloaded);
	double curpercent = nowDown / curlDown->mFileLenth * 100; // 2001619

	timeb now2;
	ftime(&now2);
	double currentTime = now2.time * 1000 + now2.millitm;//毫秒
	double intervalTime = 0;
	double intervalSize = 0;
	if (curlDown->dLastTime > -0.0000001 && curlDown->dLastTime < 0.000001)
	{
		intervalTime = currentTime - curlDown->dStartTime;
		intervalSize = nowDownloaded;
	}
	else
	{
		intervalTime = currentTime - curlDown->dLastTime;
		intervalSize = nowDownloaded - curlDown->dLastSize;
	}

	if (intervalTime >= 1000)
	{
		curlDown->dLastTime = currentTime;
		curlDown->dLastSize = nowDownloaded;
	}
	else
	{
		return 0;
	}
	double speed = intervalSize / intervalTime * 1000;
	std::cout << "speed:" << speed << std::endl;

	double leftTime = 0;
	if (speed > -0.0000001 && speed < 0.000001)
	{
		//pDownloader->SetRemainingTime(L"--:--:--");
	}
	else
	{
		// Time remaining
		leftTime = (curlDown->mFileLenth - nowDown) / speed;
	}
	std::cout << "leftTime:" << leftTime << std::endl;
	if (curlDown->m_pDelegate)
	{
		curlDown->m_pDelegate->onProgress(curpercent, ptr, curlDown->m_FilePath);
	}

	return 0;
}

bool CurlDown::Download() 
{
	FILE *fp = NULL;
	//if (access(m_FilePath.c_str(), 0) == 0) { // 以二进制形式追加
	//	fp = fopen(m_FilePath.c_str(), "ab+");
	//}
	//else { // 二进制写
	//	fp = fopen(m_FilePath.c_str(), "wb");
	//}
	

	if (IsFileExist(ASCII2Unicode(m_FilePath).c_str()))
	{
		fp = fopen(m_FilePath.c_str(), "ab+");
	}
	else
	{
		fp = fopen(m_FilePath.c_str(), "wb");
	}
	if (fp == NULL) {// 如果文件初始化失败进行返回
		return false;
	}

	// 读取本地文件下载大小
	long long localFileLenth = getLocalFileLength(); //已经下载的大小
	mHasDownLenth = localFileLenth;
	CURL *handle = curl_easy_init();
	std::string packageUrl = m_DownloadUrl; //下载地址+下载文件名
	curl_easy_setopt(handle, CURLOPT_URL, packageUrl.c_str()); // http://curl.haxx.se/libcurl/c/fopen.html
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, my_write_func);   //写文件回调方法
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp); // 写入文件对象
	//curl_easy_setopt(handle, CURLOPT_RESUME_FROM, localFileLenth);  // 从本地大小位置进行请求数据
	curl_easy_setopt(handle, CURLOPT_RESUME_FROM_LARGE, localFileLenth); 
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, my_progress_func); //下载进度回调方法
	curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, this); // 传入本类对象
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L); //不验证ssl

	timeb now2;
	ftime(&now2);
	dStartTime = now2.time * 1000 + now2.millitm;
	CURLcode res = curl_easy_perform(handle);
	fclose(fp);
	return res == CURLE_OK;
}

void CurlDown::downloadControler() 
{
	mFileLenth = getDownloadFileLenth(); // 获取远程文件大小
	if (mFileLenth <= 0) 
	{
		std::cout << "download file fail..." << std::endl;
		m_pDelegate->onError(kNetwork);
		return;
	}

	m_FileName = m_DownloadUrl.substr(m_DownloadUrl.rfind('/') + 1);
	
	m_FilePath = m_FilePath + m_FileName;
	
	bool ret = false;
	while (true){ // 循环下载 每30秒进行下载 避免断网情况
		ret = Download(); //直接下载 进行堵塞线程
		if (m_bStop) { // 如果进行停止 break
			break;
		}
		if (ret){ //下载完成
			break;
		}
		Sleep(0.5); //每次下载中间间隔0.5秒
	}

	if (ret) {
	
		m_pDelegate->onSuccess(m_FilePath);
	}
	else {
		
		m_pDelegate->onError(kUncompress);
	}
}

void CurlDown::setStopDown()
{
	m_bStop.exchange(true);
}