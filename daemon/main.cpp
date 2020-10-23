#include "daemon.h"
#include <string>
#include <iostream>

int main()
{
    std::string pid_file = "xxxxxx.pid";
    if(Daemon::already_running(pid_file.c_str()))
    {
        std::cout << "already running" << std::endl;
        return -1;
    }
    if (Daemon::daemonize() < 0)
    {
        fprintf(stderr, "ERROR: daemonize failed!\n");
        return -1;
    }
    if(Daemon::already_running(pid_file.c_str()))
    {
        std::cout << "already running" << std::endl;
        return -1;
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}
