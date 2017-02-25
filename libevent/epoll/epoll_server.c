/*
 *  epoll_server.c
 *  
 *  Created on: 2016年10月17日
 *	Author: weizheng
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/epoll.h>

#define SERVER_PORT (7778)
#define EPOLL_MAX_NUM (2048)
#define BUFFER_MAX_LEN (4096)

int main(int argc, char** argv) 
{
	int listen_fd = 0;
	int client_fd = 0;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	socklen_t client_len;

	int epfd = 0;
	struct epoll_event event, *my_events;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htconl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	listen(listen_fd, 10);

	epfd = epoll_create(EPOLL_MAX_NUM);

}
