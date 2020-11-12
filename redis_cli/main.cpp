#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

using RCommands = std::vector<std::string>;

template< class T >
using remove_reference_t = typename std::remove_reference<T>::type;


#define RealType(v) remove_reference_t<decltype(v)>

struct CommandBuilder
{
	template <typename T>
	CommandBuilder& operator << (const T& data)
	{
		stringStream.str("");
		stringStream << data;
		commands.push_back(stringStream.str());
		return *this;
	}

	RCommands get()
	{
		return std::move(commands);
	}

	std::ostringstream stringStream;
	RCommands commands;
};

template <typename R>
struct eval
{
	using Reply = R;

	template <typename ... ARGS>
	RCommands operator()(const std::string& script, ARGS&& ... args)
	{
		CommandBuilder command_builder;
		[](...) {} ((command_builder << std::forward<ARGS>(args), 0)...);
		command_builder << script;   // << "EVAL";
		auto cmd = command_builder.get();
		std::reverse(cmd.begin(), cmd.end());
		return std::move(cmd);
	}
};

void print(const std::vector<std::string> &cmd)
{
	for (auto& it : cmd)
	{
		std::cout << it << " ";
	}
	std::cout << std::endl;
}

template <typename Command>
class RedisCommandExecutor
{
	/*using RedisDriver = std::shared_ptr<RedisWrapper>;
	RedisDriver driver_;*/
	Command cmd_;
public:
	template<typename ...Args>
	RedisCommandExecutor(Args... args) : cmd_(std::forward<Args>(args)...) {
	}

	template<typename ...Args>
	int execute(typename Command::Reply &reply, Args... args)
	{
		auto cmd = cmd_(std::forward<Args>(args)...);

		//std::cout << cmd.size() << std::endl;
		print(cmd);
		
		
		reply = "98237498237";
		/*RedisWrapper::RedisReply redisReply = driver_->command(cmd);
		if (RedisReplyGet<typename Command::Reply>::get(redisReply, reply) < 0)
		{
			return -1;
		}*/
		return 0;
	}
};

int main()
{
	
	std::string a = "";

	RedisCommandExecutor<eval<std::string>> cmd;
	cmd.execute(a, "hahah", "hehhehhe");

	std::cout << a << std::endl;
	system("PAUSE");
	return 0;
}