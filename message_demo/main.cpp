#include "message_demo.h"
#include "message_factorys.h"

int main()
{
	Message* p = factory::get().produce("message1");
	p->foo();   //Message1
	delete p;
	
	auto p2 = factory::get().produce_unique("message2");
	p2->foo();
}