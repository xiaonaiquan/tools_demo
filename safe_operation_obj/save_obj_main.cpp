#include <iostream>
#include <mutex>
#include <memory>
#include <crtdbg.h>

#define CRTDBG_MAP_ALLOC  

using namespace std;

template < typename T>
class Singleton : private T
{
private:
	Singleton()
	{}
	~Singleton()
	{}
public:
	static T& instance()
	{
		static Singleton<T> s_obj;
		return s_obj;
	}
};

template<typename T>
class AtomicSharedPtr {
	typedef std::shared_ptr<T> shared_ptr_t;

	shared_ptr_t storage_;      // 数据（shared_ptr类型）
	std::mutex mutex_;          // 数据锁

	inline shared_ptr_t exchange(shared_ptr_t o) {
		std::lock_guard<std::mutex> guard(mutex_);
		std::cout << "ooo " << o.use_count() << std::endl;
		storage_.swap(o);
		std::cout << "storage_" << storage_.use_count() << std::endl;
		return o;
	}
public:
	~AtomicSharedPtr()
	{
		std::cout << "hhah析构" << std::endl;
	}
	inline shared_ptr_t load() {
		std::lock_guard<std::mutex> guard(mutex_);
		return storage_;
	}
	inline void store(shared_ptr_t o) {
		std::cout << "oo " << o.use_count() << std::endl;
		exchange(o);
	}
};

class ScopeGuard
{
public:
	typedef std::function<void()> Func;
	ScopeGuard(Func onExitScope) :onExitScope_(onExitScope), dismissed_(false)
	{

	}
	~ScopeGuard()
	{
		if (!dismissed_)
		{
			onExitScope_();
		}
	}
	void Dismiss()
	{
		dismissed_ = true;
	}

private:
	Func onExitScope_;
	bool dismissed_;

private:
	ScopeGuard(const ScopeGuard& other) = delete;
	ScopeGuard& operator=(const ScopeGuard& other) = delete;
};

#define SCOPEGUARD_LINENAME_CAT(name,line) name##line
#define SCOPEGUARD_LINENAME(name,line) SCOPEGUARD_LINENAME_CAT(name, line)
#define ON_SCOPE_EXIT(code) ScopeGuard SCOPEGUARD_LINENAME(EXIT,__LINE__)([&]{code;}); (void)(SCOPEGUARD_LINENAME(EXIT,__LINE__))

class AA 
{
public:
	AA()
	{
		std::cout << "构造" << std::endl;
	}
	~AA()
	{
		std::cout << "析构" << std::endl;
	}
	void print()
	{
		std::cout << "print" << std::endl;
	}
};

typedef Singleton< AtomicSharedPtr<AA> > a_ptr;

void test()
{
	int i = 5;

	std::shared_ptr<AA> prepare_dataload(new AA);
	std::cout << prepare_dataload.use_count() << std::endl;
	a_ptr::instance().store(prepare_dataload);
	std::cout << prepare_dataload.use_count() << std::endl;
	std::cout << a_ptr::instance().load().use_count() << std::endl;
	std::cout << prepare_dataload.use_count() << std::endl;
	ON_SCOPE_EXIT(prepare_dataload.reset());
	std::cout << prepare_dataload.use_count() << std::endl;
	_CrtDumpMemoryLeaks();

}

//void test2()
//{
//	std::shared_ptr<AA> prepare_dataload(new AA);
//	std::cout << prepare_dataload.use_count() << std::endl;
//	std::shared_ptr<AA> p;
//	p.swap(prepare_dataload);
//	std::cout << prepare_dataload.use_count() << std::endl;
//	std::cout << p.use_count() << std::endl;
//}

int main() 
{
	test();
	//test2();
	system("PAUSE");
	return 0;
}


