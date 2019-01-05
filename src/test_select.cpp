/*
 * test_select.cpp
 *
 *  Created on: Jan 5, 2019
 *      Author: gyd
 */
#pragma once
#ifndef SOCKET_SERVER_H_
#define SOCKET_SERVER_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
#define DEFAULT_PORT 3401
#define MAXLINE 4096

int socket_server()
{
    int socket_fd, connect_fd;
    int n;
    struct sockaddr_in servaddr;
    char buff[4096];
    // initialize
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("Create socket error: %s (errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //
    servaddr.sin_port = htons(DEFAULT_PORT);       //
    // bind
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("Bind error: %s (errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    if( listen(socket_fd, 10) == -1)
    {
        printf("Listen error: %s (errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1)
    {
        printf("Accept error: %s (errno: %d)\n", strerror(errno), errno);
    //    continue;
    }
    //
    struct timeval tv;
    fd_set rfds;
    int retval, maxfd = 0;
    while(1) {
       /*把可读文件描述符的集合清空*/
       FD_ZERO(&rfds);
       /*把标准输入的文件描述符加入到集合中*/
       FD_SET(0, &rfds);
       maxfd = 0;
       /*把当前连接的文件描述符加入到集合中*/
       FD_SET(connect_fd, &rfds);
       /*找出文件描述符集合中最大的文件描述符*/
       if(maxfd < connect_fd)
           maxfd = connect_fd;
       /*设置超时时间*/
       tv.tv_sec = 5;
       tv.tv_usec = 0;
       /*等待聊天*/
       retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
       if(retval == -1){
           printf("select出错，客户端程序退出\n");
           break;
       }else if(retval == 0){
           printf("服务端没有任何输入信息，并且客户端也没有信息到来，waiting...\n");
           continue;
       }else{
           /*客户端发来了消息*/
           if(FD_ISSET(connect_fd,&rfds)){
               char buffer[1024];
               memset(buffer, 0 ,sizeof(buffer));
               int len = recv(connect_fd, buffer, sizeof(buffer), 0);
               if(strcmp(buffer, "exit\n") == 0) break;
               printf("%s", buffer);
               //send(conn, buffer, len , 0);把数据回发给客户端
           }
           /*用户输入信息了,开始处理信息并发送*/
           if(FD_ISSET(0, &rfds)){
               char buf[1024];
               fgets(buf, sizeof(buf), stdin);
               //printf("you are send %s", buf);
               send(connect_fd, buf, sizeof(buf), 0);
           }
       }
   }




    while(true)
    {
        if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            printf("Accept error: %s (errno: %d)\n", strerror(errno), errno);
            continue;
        }

        if(!fork())
        {
            while(true)
            {
                n = recv(connect_fd, buff, MAXLINE, 0);
                buff[n] = '\0';
                if(send(connect_fd, "Hello, this is server!\n", 23, 0) == -1)
                    perror("send error");
                printf("recv msg from client: %s", buff);
                if(strcmp(buff,"exit\n")==0)
                {
                    close(connect_fd);
                    close(socket_fd);
                    printf("INFO: The child process is exit.\n");
                    exit(0);
                }
                printf("In the child process, pid is %d.\n", getpid());
            }
        }
    }
    close(socket_fd);
    return 0;
}

#endif

