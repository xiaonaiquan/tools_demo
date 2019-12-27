#include <iostream>
#include <string>
#include <unordered_map>

class ResponseStrategy
{
public:
	virtual void init_resp(std::string&) = 0;
};

class TypeErrorStrategy : public ResponseStrategy
{
public:
	void init_resp(std::string& data)
	{
		err_msg = data;
		std::cout << err_msg << std::endl;
	}

private:
	std::string err_msg;
};

class EmptyStrategy : public ResponseStrategy
{
public:
	void init_resp(std::string& data)
	{
		err_msg = data;
		std::cout << err_msg << std::endl;
	}
private:
	std::string err_msg;
};

class Handler
{
public:
	Handler()
	{
		my_Strategy.reserve(8);
		my_Strategy.emplace(0, std::make_shared<TypeErrorStrategy>());
		my_Strategy.emplace(1, std::make_shared<EmptyStrategy>());
	}

	void get_strategy(int num,std::string&& err)
	{
		std::unordered_map<int, std::shared_ptr<ResponseStrategy>>::iterator iter;
		iter = my_Strategy.find(num);
		if (iter != my_Strategy.end())
		{
			iter->second->init_resp(err);
		}
	}

private:
	std::unordered_map<int, std::shared_ptr<ResponseStrategy>> my_Strategy;
};

int main()
{
	Handler handle;
	std::string data = "ssss";
	handle.get_strategy(1, std::move(data));
	system("PAUSE");
	return 0;
}