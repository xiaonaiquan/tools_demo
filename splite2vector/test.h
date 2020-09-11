#pragma once
#include <iostream>
//#include <locale>
#include <vector>
#include <iterator>
#include <algorithm>
#include <xlocale>
#include <sstream>

using namespace std;

class Delimiter : public std::ctype<char>
{
public:
	Delimiter() : std::ctype<char>(get_table()) {}

	static void init_table()
	{
		static std::vector<std::ctype_base::mask> table_vec(std::ctype<char>::table_size, std::ctype_base::mask());

		table_vec[','] = std::ctype_base::space;
		table_vec['"'] = std::ctype_base::space;
		table_vec['['] = std::ctype_base::space;
		table_vec[']'] = std::ctype_base::space;
		table_vec['{'] = std::ctype_base::space;
		table_vec['}'] = std::ctype_base::space;
		table__ = &table_vec[0];
	}

	static std::ctype_base::mask const* get_table()
	{
		return table__;
	}
	static std::ctype_base::mask* table__;
};

template<typename T, typename D>
int split2vector(const std::string& input, std::vector<T>& output)
{
	if (input.empty())
	{
		return 0;
	}
	std::istringstream iss(input);
	iss.imbue(std::locale(std::locale(), new D()));
	std::copy(std::istream_iterator<T>(iss), std::istream_iterator<T>(), std::back_inserter(output));
	return 0;
}
