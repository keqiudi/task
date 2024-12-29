/***********************************************************************************
*              Copyright 2007 - 2010, Megaeyes. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_server.c
* Description: main model .
*
* History:
* Version   Date              Author        DefectNum    Description
* 1.1       2007-08-22   q60002125     NULL            Create this file.
***********************************************************************************/




#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdlib.h>      // for exit()
#include <unistd.h>     // for sleep()
#include <stdio.h>
#include <pthread.h>

#include "main_core.h"
#include "thread.h"



char Server_file_path[64];//服务端接收文件路径
char Client_file_path[64];//客户端发送文件路径



int main_core()
{
	int ret;

	//while(1)
	printf("客户端发送文件路径：");
	scanf("%s", Client_file_path);
	printf("服务端接收文件路径：");
	scanf("%s", Server_file_path);

	// 1. 初始化信号量


	// 2. 创建多线程：thread_create()
	ret = thread_create();
	if(ret !=0)
	{
		perror("Thread creation failed");
	}

	// 3. 等待所有线程结束
	thread_wait();

    printf("All threads finished.\n");

    return 0;
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
