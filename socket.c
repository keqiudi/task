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

#include <stdlib.h>
#include "thread.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

extern char Server_file_path[256];
extern char Client_file_path[256];

int tcp_server()
{

	printf("server start\n");
	int server_fd, connect_fd;
	struct sockaddr_in address;
	struct sockaddr_in clientAddr;
	char buffer[BUFFER_SIZE];

	int bytes_received, file_fd;

	// 1.创建服务器套接字
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// 2.绑定套接字
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	// 3.开始监听客户端连接
	if (listen(server_fd, 3) < 0) {
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}


	while(1)
	{
		// 4.等待客户端连接
		if ((connect_fd = accept(server_fd, (struct sockaddr *)&clientAddr, (socklen_t*)&clientAddr)) < 0) {
			perror("Accept failed");
			exit(EXIT_FAILURE);
		}

		printf("Server connected. Waiting for file...\n");


		// 保存到指定路径，指定文件名
		file_fd = open(Server_file_path, O_WRONLY | O_CREAT, 0644);
		if (file_fd == -1) {
			perror("File open failed");
			exit(EXIT_FAILURE);
		}

		// 接收客户端发送的文件数据
		while ((bytes_received = recv(connect_fd, buffer, BUFFER_SIZE, 0)) > 0)
		{
			if(write(file_fd, buffer, bytes_received)==bytes_received)
			{
				printf("File received successfully.\n");
			}
			else
			{
				perror("Receive failed");
			}
		}

		//客户端断开连接
		if (bytes_received == 0) {
			printf("client exit\n");
			close(connect_fd);//客户端退出,关闭连接
			close(file_fd);
		} else {
			perror("Receive failed");
			close(file_fd);
		}
	}

	close(server_fd);
	return 0;

}

int tcp_client()
{
	printf("client start\n");

	int socket_fd, file_fd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];
	int bytes_read;

	// 1.创建客户端套接字
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}

	// 2.连接到服务器
	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	printf("Connected to server. Sending file...\n");

	//打开指定路径下的文件
	file_fd = open(Client_file_path, O_RDONLY);  // 以只读模式打开要发送的文件
	if (file_fd == -1) {
		perror("File open failed");
		exit(EXIT_FAILURE);
	}

	//发送文件数据
	while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0)
	{

		if (bytes_read == -1) {
			perror("Read file failed");
			exit(EXIT_FAILURE);
		}

		if (send(socket_fd, buffer, bytes_read, 0) == bytes_read)
		{
			printf("File sent successfully.\n");
		}
		else
		{
			perror("Send failed");
			exit(EXIT_FAILURE);
		}

	}

	// 关闭文件和套接字
	close(file_fd);
	close(socket_fd);

	pthread_exit(NULL);
}


int udp_server()
{
	printf("UDP server started\n");

	int server_fd, bytes_received, file_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char buffer[BUFFER_SIZE];

	// 创建服务器套接字
	if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// 设置服务器地址
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// 绑定套接字
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on port %d...\n", PORT);

	// 打开文件以保存接收到的内容
	file_fd = open(Server_file_path,O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1) {
		perror("File open failed");
		exit(EXIT_FAILURE);
	}

	while (1) {
		// 接收客户端发送的数据
		bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
		if (bytes_received < 0) {
			perror("Receive failed");
			break;
		}

		// 写入文件
		if (write(file_fd, buffer, bytes_received) != bytes_received) {
			perror("File write failed");
			break;
		}

		printf("Received data from client.\n");

		// 假设文件结束标志是客户端发送一个特定的消息，例如"END"（可根据实际情况修改）
		if (bytes_received == 3 && strncmp(buffer, "END", 3) == 0) {
			printf("File transfer complete.\n");
			break;
		}
	}

	close(file_fd);
	close(server_fd);

	return 0;
}

int udp_client()
{
	printf("UDP client started\n");

	int socket_fd, file_fd, bytes_read;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	// 创建客户端套接字
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// 设置服务器地址
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}

	// 打开要发送的文件
	file_fd = open(Client_file_path, O_RDONLY);
	if (file_fd == -1) {
		perror("File open failed");
		exit(EXIT_FAILURE);
	}

	printf("Sending file to server...\n");

	while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
		if (bytes_read == -1) {
			perror("File read failed");
			exit(EXIT_FAILURE);
		}

		// 发送数据到服务器
		if (sendto(socket_fd, buffer, bytes_read, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) != bytes_read) {
			perror("Send failed");
			exit(EXIT_FAILURE);
		}

		printf("Sent %d bytes to server.\n", bytes_read);
	}

	// 发送"END"标志，表示文件发送完成
	const char *end_message = "END";
	sendto(socket_fd, end_message, strlen(end_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

	printf("File transfer complete.\n");

	// 关闭文件和套接字
	close(file_fd);
	close(socket_fd);

	return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */




