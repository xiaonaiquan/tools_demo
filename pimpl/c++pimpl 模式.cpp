
/*
Pimpl(pointer to implementation, 指向实现的指针)是一种常用的，用来对“类的接口与实现”进行解耦的方法
*/

/* public.h */

#ifndef PUBLIC_H_INCLUDED
#define PUBLIC_H_INCLUDED
 
class Book
{
public:
  Book();
  ~Book();
  void print();
 
private:
  // 禁止复制类
  Book(const Book&);
  const Book &operator = (const Book &);
 
  class BookImpl;  // Book实现类的前置声明
  BookImpl* pimpl;
};
 
#endif



/* private.h */
#ifndef PRIVATE_H_INCLUDED
#define PRIVATE_H_INCLUDED
 
#include "public.h"
#include <iostream>
 
class Book::BookImpl
{
public:
  void print();
 
private:
  std::string  m_Contents;
  std::string  m_Title;
};
 
#endif



     /* book.cpp */
#include "private.h"  // 我们需要调用BookImpl类的成员函数，
                      // 所以要包含BookImpl的定义头文件
#include "public.h"  // 我们正在实现Book类，所以要包含Book类
                     // 的头文件
 
Book::Book()
{
  pimpl = new BookImpl();
}
 
Book::~Book()
{
  delete pimpl;
}
 
void Book::print()
{
  pimpl->print();
}
 
/* BookImpl类的实现函数 */
 
void Book::BookImpl::print()
{
  std::cout << "print from BookImpl" << std::endl;
}

/* main.cpp */
#include "public.h"
 
int main()
{
    Book book;
    book.print();
 
    return 0;
}