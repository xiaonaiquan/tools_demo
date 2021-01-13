#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "TimeInterval.h"
#include <thread>
#include <fcntl.h>

pid_t g_pidWorker = 0;

void CallBackSignal(int signo)
{
    switch (signo)
    {
    case SIGALRM:
    case SIGUSR1:
    case SIGUSR2:
        break;
    default:
    {
        std::cout << "kill " << std::endl;
        const pid_t pid = g_pidWorker;
        if (0 < pid)
        {
            kill(pid, SIGKILL);
        }
        break;
    }
    }
}

pid_t MasterProcessCycle(char** argv,char *argcend,const std::string& name)
{
    const pid_t pid = fork();
    if (0 == pid)
    {
        strncpy(argv[0], (name + ": worker process").c_str(), argcend - argv[0]);
        while(true)
        {
            std::cout << "child process" << std::endl;
            sleep(1);
        }
    }
    return pid;
}

void LaunchDaemon()
{
    signal(SIGCHLD, SIG_IGN);
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(-1);
    }
    else if (pid > 0)
    {
        exit(0);
    }
    else
    {
        setsid();
        int fd;
        fd = open("/dev/null", O_RDWR, 0);
        if (fd != -1)
        {
            dup2(fd, STDIN_FILENO);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            if (fd > 2)
                close(fd);
        }
    }
}

int main(int argc, char *argv[])
{
    char **ppArgv = nullptr;
    std::string strBinName = argv[0];
    const char *const psz = strrchr(argv[0], '/');
    if (nullptr != psz)
    {
        strBinName = std::string(psz + 1);
    }
    char *pArgvEnd = argv[0];
    std::string strCmdline = "";
    for (unsigned int uiIndex = 0; nullptr != argv[uiIndex]; uiIndex++)
    {
        if (pArgvEnd == argv[uiIndex])
        {
            strCmdline += strCmdline.empty() ? "" : " ";
            strCmdline += argv[uiIndex];
            pArgvEnd = argv[uiIndex] + strlen(argv[uiIndex]) + 1;
        }
    }
    for (unsigned int uiIndex = 0; nullptr != environ[uiIndex]; uiIndex++)
    {
        if (pArgvEnd == environ[uiIndex])
        {
            pArgvEnd = environ[uiIndex] + strlen(environ[uiIndex]) + 1;
        }
    }
    if (pArgvEnd > argv[0])
        --pArgvEnd;
    char ch;
    bool bWatchDog = false;
    while ((ch = getopt(argc, argv, "w")) != -1)
    {
        switch (ch)
        {
        case 'w':
        {
            bWatchDog = true;
            break;
        }
        default:
            return 1;
        }
    }
    
    //LaunchDaemon();

    if(bWatchDog)
    {
        signal(SIGINT, CallBackSignal);
        signal(SIGKILL, CallBackSignal);
        signal(SIGTERM, CallBackSignal);

        signal(SIGUSR1, CallBackSignal);
        signal(SIGUSR2, CallBackSignal);
        strncpy(argv[0], (strBinName + ": master process " + strCmdline).c_str(), pArgvEnd - argv[0]);
        CTimeInterval timeInterval(1, 0, 60, 0);
        while (0 != (g_pidWorker = MasterProcessCycle(argv,pArgvEnd,strBinName)))
        {
            if (0 < g_pidWorker)
            {
                waitpid(-1, nullptr, 0);
                timeInterval.Reset();
            }
            struct timeval timeout = timeInterval++;
            std::this_thread::sleep_for(std::chrono::microseconds(timeout.tv_sec * 1000000 + timeout.tv_usec));
        }
        
    }
    else
    {
        while (true)
        {
            std::cout << "work process" << std::endl;
            sleep(1);
        }
        
    }
    
    return 0;
}