
#ifdef __cplusplus       
#if __cplusplus
extern "C"{                
#endif
#endif /* __cplusplus */


#include <stdio.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>             // for system()
#include <sys/reboot.h>         // for reboot(RB_AUTOBOOT)
#include <errno.h>
#include <sys/stat.h> 
#include <unistd.h>

#include "main.h"
#include "main_core.h"



int a;


int main_process()
{
	while(1)
	{
        sleep(5);//模拟主进程有对应操作
    }

	
	return 0;
}

void main()
{

    pid_t mainpid = fork();  // 创建子进程,成功会返回0

    if (mainpid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (mainpid == 0)
    {
        main_core();// 子进程中运行 main_core
    }
    else
    {
        main_process();// // 父进程中运行 main_process
    }

	return;
}





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

