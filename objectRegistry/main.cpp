#include <iostream>
#include "object_registry.h"
#include <functional>

class Base
{
public:
	virtual void print() = 0;

	virtual ~Base()    //确保子类在出作用域之后被析构
	{

	}
};

class Second : public Base
{
public:
	Second()
	{
		std::cout << "构造Second" << std::endl;
	}

	~Second()
	{
		std::cout << "~Second" << std::endl;
	}

	void print()
	{
		std::cout << "Second" << std::endl;
	}
};

typename ObjectRegistry<Base>::RegistryMap ObjectRegistry<Base>::object_register = {};

int main()
{
	{
		ObjectRegistry<Base>::register_object<Second>("ssss");

		std::shared_ptr<Base> obj(ObjectRegistry<Base>::get_object("ssss"));

		obj->print();

		std::cout << obj.use_count() << std::endl;

		std::cout << ObjectRegistry<Base>::object_register.size() << std::endl;

		std::cout << obj.use_count() << std::endl;
	}

	//system("PAUSE");
	return 0;
}