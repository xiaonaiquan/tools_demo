#pragma once
#include <stdio.h>   
#include <cstdlib>   
#include <iostream>   
#include <boost/thread.hpp>  
#include <boost/asio.hpp>

#include <map>
#include <boost/serialization/singleton.hpp>


namespace base {
class io_service_pool {

public:
    io_service_pool();
    virtual ~io_service_pool(void);
    void run(size_t thread_size);
    void stop();
public:
    // 获取ioservice，用于post任务
    boost::asio::io_service& get_io_service();
private:
    typedef std::shared_ptr<boost::asio::io_service::work> work_ptr;
    typedef std::shared_ptr<boost::thread> thread_ptr;
    boost::asio::io_service io_service_;
    work_ptr work_;
    std::vector<thread_ptr> threads_;
    size_t thread_size_;

};

typedef boost::serialization::singleton<io_service_pool> the_service_pool;
}
