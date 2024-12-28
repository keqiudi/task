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

#include "thread.h"
#include <stdio.h>
#include "socket.h"


s_mapTh2Hnd g_mapTh2Hnd[]={
    {1,	0,tcp_server},
    {1,	0,tcp_client},
    {1,	0,udp_server},
	{1, 0,udp_client},
};

pthread_t thread_id[4];

int thread_create()
{

    // // 创建tcp服务器线程
    // if (pthread_create(&thread_id[1], NULL, (void *(*)(void *))g_mapTh2Hnd[0].thread_Handler, NULL) != 0) {
    //     perror("Server thread creation failed");
    //     return -1;
    // }
    //
    // // 创建tcp客户端线程
    // if (pthread_create(&thread_id[2], NULL, (void *(*)(void *))g_mapTh2Hnd[1].thread_Handler, NULL) != 0) {
    //     perror("Client thread creation failed");
    //     return -1;
    // }

    // 创建udp服务器线程
    if (pthread_create(&thread_id[3], NULL, (void *(*)(void *))g_mapTh2Hnd[2].thread_Handler, NULL) != 0) {
        perror("Server thread creation failed");
        return -1;
    }

    // 创建udp客户端线程
    if (pthread_create(&thread_id[4], NULL, (void *(*)(void *))g_mapTh2Hnd[3].thread_Handler, NULL) != 0) {
        perror("Client thread creation failed");
        return -1;
    }

	return 0;
}

// 等待线程结束
int thread_wait() {
    // 等待所有线程完成
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    pthread_join(thread_id[2], NULL);
    pthread_join(thread_id[3], NULL);
}







#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */




