#include <iostream>
#include <cstdlib>
using namespace std;
//#define FOO_NON_DEFAULT_CONSTRUCT
class A
{
public:
	A() :a(0) { cout << "construct..." << endl; }
	A(int value) :a(value) { cout << "construct..." << endl; }
	~A() { cout << "deconstruct..." << endl; }
	void print() { cout << a << endl; }
private:
	int a;
};
class B
{
public:
	B() :b(0) { cout << "construct..." << endl; }
	B(int value) :b(value) { cout << "construct..." << endl; }
	virtual ~B() { cout << "deconstruct..." << endl; }
	void print() { cout << b << endl; }
private:
	int b;
};
class foo
{
public:
	int val;
	//foo * p_next;
	//编译器报错，没有合适默认构造函数
	foo(int a) { a = val; }
};
class Foo
{
public:
	int val;
	Foo * p_next;
	Foo() :val(0), p_next(NULL) {}
};
class Bar
{
public:
	Foo f_obj;
	void print() { cout << "(Bar.)b = " << b << endl; }
private:
	int b;
};
class Foo2 : public Foo
{
public:
	int f2;
};
class Foo3
{
public:
	virtual void print() { cout << "Foo3.f3 = " << f3 << endl; }
private:
	int f3;
};
class Foo4
{
public:
	Foo4() :b(0) {}
private:
	static int a;
	int b;
};
void use_malloc_free()
{
	cout << "use_malloc_free" << endl;
	A *a = (A *)malloc(sizeof(A));
	free(a);
}
void use_new_delete()
{
	cout << "use_new_delete" << endl;
	A *a = new A();
	delete a;
}
void fun(void)
{
	cout << "after main" << endl;
	system("pause");
}
//编译时决定数组大小，所以这样给出数组大小，会造成编译错误
#if 0
void foo(int size)
{
	int a[size];
}
#endif
int Foo4::a = 1;//类static成员的初始化
int main()
{
	A obj_a;
	//对于对象的非静态成员函数，编译器会自动添加形参this，this==&obj_a
	obj_a.print();
	//关于类对象的大小：
	//1、对于一个没有虚拟函数的对象，它的大小仅仅包括非静态数据成员的大小，其计算方法同C中的struct
	//2、如果一个类对象包含虚拟函数，则编译器自动为该类对象添加一个指向虚函数表（vtbl）的指针（vptr），
	//   故需在1的基础上加4个字节大小
	cout << "sizeof(A) = " << sizeof(A) << endl;
	cout << "sizeof(B) = " << sizeof(B) << endl;
	//编译器自动合成默认构造函数
	//1、对于一个类对象，如果该类定义了含有参数的构造函数，并没有包含一个含有构造函数的类成员（member object），
	//   那么编译器不会自动构造一个合成默认构造函数，在这种情况下，对象无法无参正常实例化。
#ifdef FOO_NON_DEFAULT_CONSTRUCT
	foo f_obj;
	cout << "f_obj.val is "<< f_obj.val << endl;
#endif
	//2、对于一个类对象，该类包含一个含有构造函数的类成员，如果它并程序员没有定义一个构造函数，
	//   编译器将会为它自动生成一个合成的构造函数，目的是为了初始化这个（这些）类成员，这时，类对象
	//   的其他成员讲保持未初始化状态，但构造函数已经为之申请了内存空间。
	Bar b_obj;
	b_obj.print();
	cout << "b_obj.f_obj.val = " << b_obj.f_obj.val << endl;
	//3、如果一个类对象，程序员并没有为它编写默认构造函数，但是它衍生于一个拥有默认构造函数的类，
	//   那么编译器将自动为之合成默认构造函数，负责构造它的基类，而对其他数据成员不做任何处理。
	Foo2 f2_obj;
	cout << "f2_obj.val = " << f2_obj.val << endl;
	//4、如果一个类对象，程序员并没有为它编写默认构造函数，但是它含有虚拟函数，
	//   那么编译器将自动为之合成默认构造函数，负责生成虚拟函数表，并初始化vptr，而对其他数据成员不做任何处理。
	Foo3 f3_obj;
	f3_obj.print();
	//Foo4 f4_obj;
	//f4_obj.a = 1;

	//cout << Foo4::a << endl;
	atexit(fun);//注册终止函数
	use_malloc_free();
	use_new_delete();
}