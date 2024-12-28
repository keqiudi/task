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


#include <pthread.h>


typedef struct tagthread2handler
{
	int start_flag;				// 0, 1启动
	int thread_quit_flag;       // 线程主动退出标志,置1时退出该线程
	void *thread_Handler;
} s_mapTh2Hnd;




int thread_create();
int thread_wait();


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */




