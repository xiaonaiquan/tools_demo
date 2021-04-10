#include <iostream>
#include "timer.h"

using namespace std;

void test_run_at_func(chrono::time_point<chrono::steady_clock> t1, int ms)
{
	std::thread::id tid = std::this_thread::get_id();

	auto t2 = chrono::steady_clock::now();
	int tm_diff = static_cast<int>(chrono::duration<double, milli>(t2 - t1).count());
	//LOG_INFO("[tid:%lld] hello, run_at_func ! run at %d ms, time diff is %d ms\n", tid, ms, tm_diff);
	cout << "f id=" << tid << endl;
}

int main()
{
	Timer t;
	t.run();

	int run_at_ms = 1000;
	auto t1 = std::chrono::steady_clock::now();
	auto now = std::chrono::high_resolution_clock::now();
	//LOG_INFO("[tid:%lld] start to test run at certain time, run at %d ms after now\n", tid, run_at_ms);
	auto certion_id = t.run_at(now + std::chrono::milliseconds(run_at_ms), test_run_at_func, t1, run_at_ms);
	system("PAUSE");
	return 0;
}