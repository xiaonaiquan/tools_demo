#include "asio_timer_base.h"

asio_timer::asio_timer():io_service_() {
    if(!io_thread_)
    {
        io_worker_ = std::make_shared<boost::asio::io_service::work>(io_service_);
        io_thread_ = std::make_shared<boost::thread>(std::bind(&asio_timer::io_main_thread,this));
		/*if (!io_thread_back_) {
		io_thread_back_ = std::make_shared<boost::thread>(std::bind(&asio_timer::io_main_thread,this));
		}*/
    }

}
asio_timer::~asio_timer() {
    io_worker_.reset();
    io_service_.stop(); //强制结束
    io_thread_.reset();
    io_thread_back_.reset();
}
bool asio_timer::set_timer(int timer_id,int elapse,timer_callback callback,bool run_once) {
	//boost::lock_guard<boost::mutex> guard(mtx_);
    auto iter = map_timer_.find(timer_id);
    if (iter == map_timer_.end()) {
		timer_info_t_ptr timer_info = std::make_shared<timer_info_t>();
		timer_info->timer_id_ = timer_id;
		timer_info->timer_elapse_ = elapse;
		timer_info->call_back_ = callback;
        if (run_once)
        {
            // 如果立即执行
            io_service_.post([timer_info]()
            {
                boost::system::error_code error;
                timer_info->call_back_(error);
            });
        }
        
        timer_info->timer_ = std::make_shared<boost::asio::steady_timer>(io_service_);
        timer_info->timer_->expires_from_now(boost::chrono::milliseconds(elapse));
        timer_info->timer_->async_wait(boost::bind(&asio_timer::ontimer_internal, this, timer_id, boost::asio::placeholders::error));
        map_timer_.insert(std::make_pair(timer_id,timer_info));
    }
    return true;
}
bool asio_timer::kill_timer(int timer_id) {
	
    auto iter = map_timer_.find(timer_id);
    if (iter == map_timer_.end()) {
        return false;
    }
    iter->second->timer_->cancel();
	iter->second.reset();
    map_timer_.erase(iter);
    return true;
}
void asio_timer::ontimer_internal(int timer_id, const boost::system::error_code& error) {
	if (!error)
	{
		auto iter = map_timer_.find(timer_id);
		if (iter != map_timer_.end()) {
			iter->second->timer_->expires_from_now(boost::chrono::milliseconds(iter->second->timer_elapse_));
			iter->second->timer_->async_wait(boost::bind(&asio_timer::ontimer_internal, this, iter->second->timer_id_, boost::asio::placeholders::error));
			iter->second->call_back_(error);
		}
	}
	
	
}
void asio_timer::io_main_thread()
{
    for (;;)
    {
        try
        {
            io_service_.run();
            break; // run() exited normally
        }
        catch (std::exception& e)
        {
            error_ = e.what();
        }
        catch (...)
        {
            
        }
    }
}