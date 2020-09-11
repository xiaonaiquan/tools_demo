#include "test.h"

std::ctype_base::mask* Delimiter::table__ = 0;

struct InitTable
{
	InitTable()
	{
		Delimiter::init_table();
	}

} InitTable;

int main()
{
	std::string btype_array = "aaabccc112344";
	std::vector<std::string> v;
	split2vector<std::string, Delimiter>(btype_array, v);

	for (auto &it : v)
	{
		std::cout << it << std::endl;
	}
	system("PAUSE");
	return 0;
}