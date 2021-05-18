/*
* 程序喵大人：函数耗时
*/

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <chrono>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <thread>

using llong = long long;

#define CAL_SCOPE_TIME(x) TimeLog t(x);
#define CAL_TIME_BEGIN(x) auto begin_##x = TimeLog::Now();
#define CAL_TIME_END(x) \
std::cout << "time { " << #x << " } " << TimeLog::DiffMs(begin_##x, TimeLog::Now()) << "ms" << std::endl;

int gettimeofday(struct timeval* tp, void* tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}

class TimeLog
{
public:
	TimeLog(const std::string tag)
	{
		m_begin_ = std::chrono::high_resolution_clock::now();
		m_tag_ = tag;
	}

	~TimeLog()
	{
		auto time =
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_begin_).count();
		std::cout << "time { " << m_tag_ << " } " << static_cast<double>(time) << " ms" << std::endl;
	}

	void Reset() 
	{ 
		m_begin_ = std::chrono::high_resolution_clock::now(); 
	}

	llong Elapsed()
	{
		return static_cast<llong>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_begin_).count());
	}

	static std::chrono::time_point<std::chrono::high_resolution_clock> Now() 
	{
		return std::chrono::high_resolution_clock::now();
	}

	static llong DiffUs(std::chrono::time_point<std::chrono::high_resolution_clock> before,
		std::chrono::time_point<std::chrono::high_resolution_clock> after) 
	{
		return static_cast<llong>(std::chrono::duration_cast<std::chrono::microseconds>(after - before).count());
	}

	static llong DiffMs(std::chrono::time_point<std::chrono::high_resolution_clock> before,
		std::chrono::time_point<std::chrono::high_resolution_clock> after) 
	{
		return static_cast<llong>(std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count());
	}

	static llong GetCurrentMs()
	{
		struct timeval time;
		gettimeofday(&time, NULL);
		return static_cast<llong>(time.tv_sec ) * 1000 + static_cast<llong>(time.tv_usec / 1000);
	}

	static void ShowCurTime() 
	{
		time_t now = time(0);
		char* dt = ctime(&now);
		std::cout << "cur time is " << dt << std::endl;
		std::cout << "cur ms is " << GetCurrentMs() << std::endl;
	}

	static struct timeval GetCurrentTimeofDay() 
	{
		struct timeval time;
		gettimeofday(&time, NULL);
		return time;
	}

private:
	std::string m_tag_;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_begin_;
};

void TestTimerLog() {
	auto func = []() {
		for (int i = 0; i < 5; ++i) 
		{
			std::cout << "i = " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	};
	{
		CAL_SCOPE_TIME("func time");
		func();
	}

	{
		CAL_TIME_BEGIN(func123)
		func();
		CAL_TIME_BEGIN(func234)
		func();
		func();
		CAL_TIME_END(func123)
		func();
		CAL_TIME_END(func234)
	}
	
}

int main()
{
	TestTimerLog();
	return 0;
}