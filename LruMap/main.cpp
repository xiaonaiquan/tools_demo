#include "lurmap.h"
#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	LruMap<string, int> test;
	test.put("xiao", 1 ,2);
	test.put("ene", 2, 2);
	std::cout << "fang ru" << std::endl;
	Sleep(5000);
	/*for (auto & it : test)
	{
		std::cout << it.key.c_str() << " " ;
	}*/
	auto it = test.get("xiao");   //  如果过期,需要判断返回的it,返回可以设置返回空
	system("PAUSE");
	return 0;
}

