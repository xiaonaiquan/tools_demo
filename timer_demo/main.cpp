#include "asio_timer_base.h"
#include <iostream>


int main()
{
	int i = 0;
	the_asio_timer::get_mutable_instance().set_timer(timer_query_user_state, 1000, [&i](const boost::system::error_code& err){
		
		i++;
		std::cout << i << std::endl;
		if (i > 9)
		{
			the_asio_timer::get_mutable_instance().kill_timer(timer_query_user_state);
		}
	});
	system("PAUSE");
	return 0;
}