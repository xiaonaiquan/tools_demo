#include <iostream>
#include <memory>
#include <mutex>
#include <functional>
#include <map>

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


using std::shared_ptr;
using std::make_shared;
using std::lock_guard;
using std::mutex;
using std::function;

template<typename T>
class ThreadSafeElem {
public:
	typedef shared_ptr<T> safe_type;

	typedef function<void(safe_type& inner)> adaptor_functype;

public:
	ThreadSafeElem()
	{
		_elem = make_shared<T>();
	}
	inline void safe_operation(adaptor_functype func_in_lock)
	{
		lock_guard<mutex> guard(_mutex);
		func_in_lock(_elem);
	}
private:
	safe_type  _elem;
	mutex _mutex;
};

class A
{
public:
	A()
	{
		cout << "构造" << endl;
	}
	~A()
	{
		cout << "析构" << endl;
	}

	void print()
	{
		cout << "print" << endl;
	}
};

typedef Singleton<ThreadSafeElem<A>> g_A;

int main()
{
	g_A::instance().safe_operation(
		[](ThreadSafeElem<A>::safe_type &inner) {
		inner->print();
		cout << "safe_operation" << endl;
	});

	system("PAUSE");
	return 0;
}