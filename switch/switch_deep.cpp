/*************************************************************************
	> File Name: switch_deep.cpp
	> Author: ryan_naiquan
	> Mail: ryan_naiquan@163.com 
	> Created Time: Mon 16 Sep 2019 02:37:24 PM CST
 ************************************************************************/

#include<iostream>
#include<string>
#include<stdio.h>
#include "switch_deep.h"

class User {
 public:
  explicit User(int age) : age_(age) {}

  //重载<是因为mapstd::map<>::find不是通过==进行查找的，而是<，因此必须重载<
  //std::map<>::find不是通过==进行查找的，而是<，因此必须重载<
  bool operator<(const User& user) const { return this->age_ < user.age(); }

  int age() const { return age_; }

 private:
  int age_;
};

/*
  User u1(20), u2(22), ux(20);
  select(ux)
    .found(u1, []() {
      printl("it's u1");
    })
    .found(u2, []() {
      printl("it's u2");
    }).done();
  // it's u2
*/

int main()
{
	std::string condition("windows");

  // match std::string
  program_options::select(condition)
    .found("apple", []() {
      printf("it's apple");
    })
    .found("windows", []() {
      printf("it's windows");
    })
    .found("linux", []() {
      printf("it's linux");
    }).done();
	return 0;
}



