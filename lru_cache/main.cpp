#include <iostream>
#include "lrucache.h"
#include <string>
int main()
{
	lru_cache<std::string, std::string> l1(2);


	l1.insert("a", "111");
	l1.insert("b", "111");

	l1.insert("c", "111");

	std::optional<std::string> bb= l1.get("b");

	if (bb != "")
	{
		std::cout << "find" << std::endl;
	}
	else
	{
		std::cout << "not find" << std::endl;
	}
	return 0;
}