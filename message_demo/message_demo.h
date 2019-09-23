/*************************************************************************
	> File Name: message_demo.h
	> Author: ryan_naiquan
	> Mail: ryan_naiquan@163.com 
	> Created Time: Mon 23 Sep 2019 01:49:43 PM CST
 ************************************************************************/

#pragma once

#include "message_factorys.h"
#include "message_base.h"

class Message1 : public Message
{
public:

	Message1()
	{
		std::cout << "message1 构造函数" << std::endl;
	}

	Message1(int a)
	{
		std::cout << "message1 a" << std::endl;
	}

	~Message1()
	{
	}

	void foo() override
	{
		std::cout << "message1 foo成员函数" << std::endl;
	}
};

//REGISTER_MESSAGE(Message1, "message1", 2);
REGISTER_MESSAGE(Message1, "message1");

class Message2 : public Message
{
public:

	Message2()
	{
		std::cout << "message2 构造函数" << std::endl;
	}

	Message2(int a)
	{
		std::cout << "message2 a" << std::endl;
	}

	~Message2()
	{
	}

	void foo() override
	{
		std::cout << "message2 foo成员函数" << std::endl;
	}
};

REGISTER_MESSAGE(Message2, "message2");

