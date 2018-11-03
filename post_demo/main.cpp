#include "asio_post_base.h"
#include <iostream>


int main()
{
	base::io_service_pool &t = base::the_service_pool().get_mutable_instance();
	t.get_io_service().post([](){
		std::cout << "asdasda" << std::endl;
	});
	t.get_io_service().run();
	system("PAUSE");
	return 0;
}