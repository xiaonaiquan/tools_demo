#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

class Daemon
{
public:
    /**
     * @brief 后台化，并重定向输入输出
     */
    static int daemonize(const char* input = NULL,
                         const char* output = NULL,
                         const char* errout = NULL)
    {
        int     fd;
        pid_t   pid;
        if ((pid = fork()) < 0)
        {
            return -1;
        }
        else if (pid)
        {///parent process terminates
            _exit(0);
        }

        /* child 1 continues... */
        if (setsid() < 0)
        {
            return -1;
        }
        signal(SIGHUP, SIG_IGN);
        if ((pid = fork()) < 0)
        {
            return -1;
        }
        else if (pid)
        {///child 1 terminates
            _exit(0);
        }

        /* child 2 continues... */
        for (fd = 0; fd <= 2; fd++)
        {
            close(fd);
        }

        fd = input ? ::open(input, O_RDONLY)
                   : ::open("/dev/null", O_RDONLY);
        if (fd < 0)
        {
            return -1;
        }

        fd = output ? ::open(output, O_RDWR | O_CREAT | O_APPEND, 0644)
                    : ::open("/dev/null", O_RDWR);
        if (fd < 0)
        {
            close(0);
            return -1;
        }

        fd = errout ? ::open(errout, O_RDWR | O_CREAT | O_APPEND, 0644)
                    : ::open("/dev/null", O_RDWR);
        if (fd < 0)
        {
            close(0);
            close(1);
            return -1;
        }

        /// set signal actions
        signal(SIGPIPE, SIG_IGN);
        return 0;
    }

    /**
     * @brief 获取pid_file中的pid
     *        该pid_file由那个进程的alreadyRunning()预先生成
     * @return >1, -1
     * @retval >1  success, pid
     * @retval -1  failure
     */
    static pid_t get_pid(const char* pid_file)
    {
        assert(pid_file);
        int fd = open(pid_file, O_RDONLY);
        if (fd < 0)
        {
            return -1;
        }
        char buf[32];
        memset(buf, 0, sizeof(buf));
        ssize_t ret = ::read(fd, buf, sizeof(buf)-1);
        if (ret <= 0)
        {
            return -1;
        }
        long pid     = 0;
        long checker = 1;
        sscanf(buf, "%ld", &pid);
        sscanf(buf, "%ld", &checker);
        if (pid != checker || pid <= 1)
        {
            return -1;
        }
        return pid;
    }

    /**
     * @brief 创建并锁定pid文件，以此来检查是否已有进程在运行
     */
    static bool already_running(const char* pid_file)
    {
        assert(pid_file);
        int fd = ::open(pid_file, O_RDWR | O_CREAT, 0644);
        if (fd < 0)
        {
            return true;    /**< 文件打开失败 */
        }
        if (lockfile(fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                close(fd);
                return true;    /**< 文件被锁住了，真正的already_running */
            }
            return true;    /**< 其他错误 */
        }
        if (ftruncate(fd, 0) < 0)
        {
            close(fd);
            return false;
        }
        char buf[32];
        sprintf(buf, "%ld", (long)getpid() );
        if ( ::write(fd, buf, strlen(buf)+1) != (ssize_t)(strlen(buf)+1) )
        {
            close(fd);
            return false;
        }
        return false;
    }

private:
    Daemon()
    {}
    /**
     * @brief 给一个文件加锁
     * @param [in] fd   文件描述符
     * @return 0, -1
     * @retval 0    success
     * @retval -1   failure
     *              IExcp::E_LOCKED     文件已被加锁，因而本次加锁失败
     */
    static int lockfile(int fd)
    {
        int             ret;
        struct flock    fl;

        fl.l_type   = F_WRLCK;
        fl.l_start  = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len    = 0;

        ret = fcntl(fd, F_SETLK, &fl);
        if (ret < 0)
        {
            return -1;
        }
        return 0;
    }
};