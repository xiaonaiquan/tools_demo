#include <iostream>
#include <memory>

using mqtt_data_type = int;

class operation
{
public:
	bool invoke(mqtt_data_type* mqdata)
	{
		return func_(this, mqdata);
	}

protected:
	using func_type = bool(*)(operation*, mqtt_data_type*);
	operation(func_type func)
		:func_(func)
	{
	}
	~operation()
	{
	}
private:
	func_type func_;
};

template <typename Handler>
class my_op :public operation
{
public:
	my_op(Handler& handler)
		:operation(&my_op::do_complete)
		, handler_(std::move(handler))
	{
	}

	static bool do_complete(operation* base, mqtt_data_type* mqdata)
	{
		// Take ownership of the operation object.
		my_op* o(static_cast<my_op*>(base));
		o->handler_(mqdata);
		return true;
	}
private:
	Handler handler_;
};

//mqtt底层可能的实现
void set_mqtt_callback(void(*fn)(mqtt_data_type* data, void*), void* ud)
{
	//模拟调用
	mqtt_data_type* data = new mqtt_data_type(100);
	fn(data, ud);
}

void my_mqtt_callback(mqtt_data_type* data, void* ud)
{
	operation* op = static_cast<operation*>(ud);
	op->invoke(data);
}

//C风格回调转换成 C++风格
void test()
{
	using func_type = void(*)(mqtt_data_type*);

	func_type f = [](mqtt_data_type* data) {
		std::cout << "result:" << *data << std::endl;
	};

	std::shared_ptr<std::string> self = std::make_shared<std::string>();
	my_op<func_type>* op = new my_op<func_type>(f);
	set_mqtt_callback(my_mqtt_callback, op);

}


int main()
{
	test();
	system("PAUSE");
	return 0;
}
