#pragma once
#define __STDC_LIMIT_MACROS
#include <boost/thread.hpp>  
#include <boost/asio.hpp>
#include <map>
#include "asio_timer_def.h"
#include "../common/Singleton.h"
#define  GENERATING_DOCUMENTATION
#include <boost/asio/steady_timer.hpp>
#include <string>

typedef std::function<void (const boost::system::error_code&)> timer_callback ;
// boost异步等待timer,不可在多线程中调用，未对数据进行加锁
class asio_timer {
protected:
    
    typedef struct timer_info_t {
        timer_info_t():timer_id_(0),timer_elapse_(0),cancel_(false) {

        }
        int timer_id_;
        int timer_elapse_;
		bool cancel_;
        timer_callback call_back_;
        std::shared_ptr<boost::asio::steady_timer> timer_;
    };
    typedef std::shared_ptr<timer_info_t> timer_info_t_ptr;
    typedef std::map<int, timer_info_t_ptr> timer_ptr_map;
public:
    asio_timer();
    ~asio_timer();
    
    bool set_timer(int timer_id,int elapse, timer_callback callback,bool run_once = false);
    bool kill_timer(int timer_id);
private:
	boost::mutex mtx_;
    boost::asio::io_service io_service_; 
    std::shared_ptr<boost::thread> io_thread_;
    std::shared_ptr<boost::thread> io_thread_back_;
    std::shared_ptr<boost::asio::io_service::work> io_worker_;
    typedef std::shared_ptr<boost::asio::io_service::work> work_ptr;
    typedef std::shared_ptr<boost::thread> thread_ptr;
    void ontimer_internal(int iTimerID, const boost::system::error_code& error);
    void io_main_thread();
    timer_ptr_map map_timer_;
	//timer_ptr_map::iterator find(int timer_id);
	std::string error_;
};
typedef Singleton<asio_timer> the_asio_timer;