#include <iostream>
#include "status.h"
#include "arena.h"

class test
{
public:
	Status pr()
	{
		return Status::IOError("ss","jajaja");
	}
};

int main()
{
	/*test t;
	std::cout << t.pr().ToString().c_str() << std::endl;*/
	Arena arena_;
	char *a = arena_.Allocate(400);

	std::cout << "a = " << strlen(a) << std::endl;

	char *b = arena_.Allocate(300);

	std::cout << "b = " << strlen(b) << std::endl;

	std::cout << "total = " << arena_.MemoryUsage() << std::endl;
	return 0;
}