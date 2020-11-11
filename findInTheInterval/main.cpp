#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class AttrType>
struct IpSegment
{
	unsigned int begin = 0;
	unsigned int end = 0;
	AttrType attr;
	IpSegment() :begin(0), end(0) {};
	IpSegment(unsigned int b, unsigned int e) :begin(b), end(e) {};
	bool operator<(unsigned int ip) const { return end < ip; }

	bool operator<(const IpSegment &o) const { return end < o.begin; }
	bool overlap(const IpSegment &o) const { return !(*this < o || o < *this); }
	const IpSegment Intersect(const IpSegment &o) {
		if (overlap(o))
			return IpSegment(
				std::max(begin, o.begin),
				std::min(end, o.end));
		return IpSegment(0, 0);
	}
};

typedef IpSegment<std::string> IpInfo;

void test()
{
	std::vector<IpInfo> data;

	IpInfo one;
	one.begin = 15;
	one.end = 19;
	one.attr = "one";


	IpInfo two;
	two.begin = 11;
	two.end = 14;
	two.attr = "two";

	
	data.push_back(two);
	data.push_back(one);

	auto it = std::lower_bound(data.begin(), data.end(), 13);
	
}

int main()
{
	test();
	system("PAUSE");
	return 0;
}