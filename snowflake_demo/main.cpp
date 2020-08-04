#include "snowflake.h"

int main()
{
	Snowflake <false> sn;
	int64_t qq = sn.generate();
	std::cout << qq << std::endl;
	system("PAUSE");
	return 0;
}