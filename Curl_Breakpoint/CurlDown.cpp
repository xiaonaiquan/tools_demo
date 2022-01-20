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


/* �õ�Զ���ļ��Ĵ�С, Ҫ���ص��ļ���С */ // �ο����Ǹ����� ��ȡ������С
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
	curl_easy_setopt(handle, CURLOPT_HEADER, 0L); // 0 ����ӡ��־ 1��ӡ��־

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
	// ������ֵ ����ע�͵�������������Ͳ�׼ȷ��
	CurlDown* curlDown = (CurlDown*)ptr;
	if (!curlDown || curlDown->mFileLenth == 0 || nowDownloaded == 0) return 0;
	double nowDown = (curlDown->mFileLenth - totalToDownload + nowDownloaded);
	double curpercent = nowDown / curlDown->mFileLenth * 100; // 2001619

	timeb now2;
	ftime(&now2);
	double currentTime = now2.time * 1000 + now2.millitm;//����
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
	//if (access(m_FilePath.c_str(), 0) == 0) { // �Զ�������ʽ׷��
	//	fp = fopen(m_FilePath.c_str(), "ab+");
	//}
	//else { // ������д
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
	if (fp == NULL) {// ����ļ���ʼ��ʧ�ܽ��з���
		return false;
	}

	// ��ȡ�����ļ����ش�С
	long long localFileLenth = getLocalFileLength(); //�Ѿ����صĴ�С
	mHasDownLenth = localFileLenth;
	CURL *handle = curl_easy_init();
	std::string packageUrl = m_DownloadUrl; //���ص�ַ+�����ļ���
	curl_easy_setopt(handle, CURLOPT_URL, packageUrl.c_str()); // http://curl.haxx.se/libcurl/c/fopen.html
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, my_write_func);   //д�ļ��ص�����
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp); // д���ļ�����
	//curl_easy_setopt(handle, CURLOPT_RESUME_FROM, localFileLenth);  // �ӱ��ش�Сλ�ý�����������
	curl_easy_setopt(handle, CURLOPT_RESUME_FROM_LARGE, localFileLenth); 
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, my_progress_func); //���ؽ��Ȼص�����
	curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, this); // ���뱾�����
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L); //����֤ssl

	timeb now2;
	ftime(&now2);
	dStartTime = now2.time * 1000 + now2.millitm;
	CURLcode res = curl_easy_perform(handle);
	fclose(fp);
	return res == CURLE_OK;
}

void CurlDown::downloadControler() 
{
	mFileLenth = getDownloadFileLenth(); // ��ȡԶ���ļ���С
	if (mFileLenth <= 0) 
	{
		std::cout << "download file fail..." << std::endl;
		m_pDelegate->onError(kNetwork);
		return;
	}

	m_FileName = m_DownloadUrl.substr(m_DownloadUrl.rfind('/') + 1);
	
	m_FilePath = m_FilePath + m_FileName;
	
	bool ret = false;
	while (true){ // ѭ������ ÿ30��������� ����������
		ret = Download(); //ֱ������ ���ж����߳�
		if (m_bStop) { // �������ֹͣ break
			break;
		}
		if (ret){ //�������
			break;
		}
		Sleep(0.5); //ÿ�������м���0.5��
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