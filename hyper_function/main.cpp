#include <iostream>
#include <functional>
#include <vector>
#include "hyper_function.h"

//原始版本
//template <typename Request, typename Response>
//class BaseCgi {
//public:
//	BaseCgi(Request request, std::function<void(Response &)> &callback) {
//		_request = request;
//		_callback = callback;
//	}
//
//	void onRequest(std::vector<uint8_t> &outData) {
//		_request.toData(outData);
//	}
//
//	void onResponse(std::vector<uint8_t> &inData) {
//		Response response;
//		response.fromData(inData);
//		callback(response);
//	}
//
//public:
//	Request _request;
//	std::function<void(Response &)> _callback;
//};
//
//class CgiA : public BaseCgi<RequestA, ResponseA> {
//public:
//	CgiA(RequestA &request, std::function<void(ResponseA &)> &callback) :
//		BaseCgi(request, callback) {}
//};


//优化的版本
class BaseRequest {
public:
	virtual void toData(std::vector<uint8_t> &outData) = 0;
};

class BaseResponse {
public:
	virtual void fromData(std::vector<uint8_t> &outData) = 0;
};

class BaseCgi {
public:
	template <typename Request, typename Response>
	BaseCgi(Request &request, hyper_function<void(Response &)> callback) {
		_request = new Request(request);
		_response = new Response;
		_callback = callback;
	}

	void onRequest(std::vector<uint8_t> &outData) {
		_request->toData(outData);
	}

	void onResponse(std::vector<uint8_t> &inData) {
		_response->fromData(inData);
		_callback(*_response);
	}

public:
	BaseRequest *_request;
	BaseResponse *_response;
	hyper_function<void(BaseResponse &)> _callback;
};

class RequestA : public BaseRequest 
{ 
public:
	void toData(std::vector<uint8_t> &outData)
	{

	}
};

class ResponseA : public BaseResponse 
{ 
public:
	void fromData(std::vector<uint8_t> &outData)
	{

	}
};

class CgiA : public BaseCgi {
public:
	CgiA(RequestA &request, hyper_function<void(ResponseA &)> &callback) :
		BaseCgi(request, callback) //子类function对象给父类
	{
	}
};

int main()
{
	system("PAUSE");
	return 0;
}