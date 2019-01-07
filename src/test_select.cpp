/*
 * test_select.cpp
 *
 *  Created on: Jan 5, 2019
 *      Author: gyd
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
#define DEFAULT_PORT 3402
#define MAXLINE 4096
void thread_fun(int socket_fd);
int socket_server()
{
    int socket_fd;
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
    std::thread th1{thread_fun, socket_fd};
    th1.join();
    close(socket_fd);
    return 0;
}
//
void thread_fun(int socket_fd)
{
    int conns[2] = {};
    int z = 0;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    memset(&client_addr, 0, client_len);

    //成功返回非负描述字，出错返回-1
    while(true)
    {
        int conn = accept(socket_fd, (struct sockaddr*)&client_addr, &client_len);
        if( conn < 0 ) {
            perror("connect");
            exit(1);
        }
        //把连接保存到临时数组中;
        conns[z] = conn;
        z++;

        fd_set rfds;
        struct timeval tv;
        int retval, maxfd;
        while(1) {
            FD_ZERO(&rfds);     //把可读文件描述符的集合清空
            FD_SET(0, &rfds);   //把标准输入的文件描述符加入到集合中
            maxfd = 0;
            FD_SET(conn, &rfds);//把当前连接的文件描述符加入到集合中
            if(maxfd < conn)    //找出文件描述符集合中最大的文件描述符
                maxfd = conn;
            /*设置超时时间*/
            tv.tv_sec = 5;
            tv.tv_usec = 0;

            retval = select(maxfd+1, &rfds, &rfds, NULL, &tv);
            if(retval == -1){
                printf("select出错，客户端程序退出\n");
                break;
            }else if(retval == 0){
                printf("服务端没有任何输入信息，并且客户端也没有信息到来，waiting...\n");
                continue;
            }else
            {
                /*客户端发来了消息*/
                if(FD_ISSET(conn,&rfds))
                {
                    char buffer[1024];
                    memset(buffer, 0 ,sizeof(buffer));
                    int len = recv(conn, buffer, sizeof(buffer), 0);
                    {
                        if(0 == len)
                        {
                            printf("客户端程序退出\n");
                            close(conn);
                        }

                    }
                    if(strcmp(buffer, "exit\n") == 0) break;
                    printf("%s\n", buffer);
                    //send(conn, buffer, len , 0);把数据回发给客户端
                }
                /*用户输入信息了,开始处理信息并发送*/
                if(FD_ISSET(0, &rfds))
                {
                    char buf[1024];
                //    fgets(buf, sizeof(buf), stdin);
                    sprintf(buf, "hello");
                    //printf("you are send %s", buf);
                    for(int i=0; i<z; i++)
                    {
                        int ret = send(conns[i], buf, sizeof(buf), 0);
                        if( ret < 0 )
                        {
                            printf("ERROR: send msg error: %s(errno: %d)\n", strerror(errno), errno);
                        }

                    }
                }
            }
        } // end of second while

    } // end of first while

    close(socket_fd);
}

