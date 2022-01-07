
#include "CurlDown.h"

class DownTest : public CurlDownDelegate, public std::enable_shared_from_this<DownTest>
{
public:
	DownTest()
	{}
	~DownTest()
	{}

	void run()
	{
		CurlDown *p = new CurlDown("xxxxxxx","D:\\");
		p->setDelegate(shared_from_this());
		p->downloadControler();
	}

	virtual void onError(CurlDown::ErrorCode errorCode) override
	{
		std::cout << "error:" << errorCode << std::endl;
	};

	virtual void onProgress(double percent, void *delegate, std::string filefullPath) override
	{
		std::cout << "percent:" << percent << std::endl;
	};
	
	virtual void onSuccess(std::string filefullPath) override
	{
		std::cout << "success:" << filefullPath.c_str() << std::endl;
	};

private:

};

int main()
{
	curl_global_init(CURL_GLOBAL_ALL);
	std::shared_ptr<DownTest> p = std::make_shared<DownTest>();
	p->run();
	curl_global_cleanup();

	system("PAUSE");
	return 0;
}