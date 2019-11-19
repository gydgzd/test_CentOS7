/*
 * test_select.cpp
 *
 *  Created on: Jan 5, 2019
 *      Author: gyd
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   // inet_pton
#include "socketDef.h"
#include "Mylog.h"
using namespace std;
#define DEFAULT_PORT 3401
#define MAXLINE 4096
void thread_fun(int socket_fd);
int myrecv( CONNECTION & client);
int logMsg(const MSGBODY *pMsg, const char *logHead, int isRecv);
int socket_server()
{
    int socket_fd;
    struct sockaddr_in servaddr;
    char buff[4096];
    // initialize
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("Create socket error: %s (errno: %d). Exit.\n", strerror(errno), errno);
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //
    servaddr.sin_port = htons(DEFAULT_PORT);       //
    //
    int optval = 1;
    if(-1 == setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
    {
        printf( "ERROR: Reuse addr error: %s (errno: %d). \nExit.", strerror(errno), errno);
        exit(-1);
        //return -1;
    }
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
    int conns[4096] = {};
    int z = 0;

    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    memset(&client_addr, 0, client_len);

    CONNECTION client;
    char logmsg[512] = "";
    Mylog mylog;
    //成功返回非负描述字，出错返回-1
    while(true)
    {
        memset(&client, 0, sizeof(client));
        client.socket_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_len);
        if( client.socket_fd < 0 ) {
            sprintf(logmsg, "Accept error: %s (errno: %d)\n", strerror(errno), errno);
            exit(1);
        }
        // get server address
        getsockname(client.socket_fd, (struct sockaddr *)&server_addr, &server_len);
        inet_ntop(AF_INET,(void *)&server_addr.sin_addr, client.serverIP, 64 );
        client.serverPort = ntohs(server_addr.sin_port);

        // get client address
        inet_ntop(AF_INET,(void *)&client_addr.sin_addr, client.clientIP, 64 );
        client.clientPort = ntohs(client_addr.sin_port);
        client.status = 1;
        sprintf(logmsg, "INFO: %s:%d --> %s:%d connected", client.clientIP, client.clientPort, client.serverIP, client.serverPort);
        mylog.logException(logmsg);
        //把连接保存到临时数组中;
        conns[z] = client.socket_fd;
        z++;

        fd_set read_fds, babckup_fds;
        struct timeval tv;
        int retval, maxfd;
        while(1) {
            FD_ZERO(&read_fds);     //把可读文件描述符的集合清空
            FD_SET(0, &read_fds);   //把标准输入的文件描述符加入到集合中
            maxfd = 0;
            FD_SET(client.socket_fd, &read_fds);//把当前连接的文件描述符加入到集合中
            if(maxfd < client.socket_fd)    //找出文件描述符集合中最大的文件描述符
                maxfd = client.socket_fd;
            /*设置超时时间*/
            tv.tv_sec = 5;
            tv.tv_usec = 0;
            read_fds = babckup_fds;
            retval = select(maxfd+1, &read_fds, NULL, NULL, &tv);
            if(retval == -1){
                printf("select出错，客户端程序退出\n");
                break;
            }else if(retval == 0){
                printf("服务端没有任何输入信息，并且客户端也没有信息到来，waiting...\n");
                continue;
            }else
            {
                /*客户端发来了消息*/
                if(FD_ISSET(client.socket_fd,&read_fds))
                {
                    char buffer[1024];
                    memset(buffer, 0 ,sizeof(buffer));
               /*     int len = recv(client.socket_fd, buffer, sizeof(buffer), 0);
                    {
                        if(0 == len)
                        {
                            printf("客户端程序退出\n");
                            close(client.socket_fd);
                            break;
                        }
                    }
                    */
                    myrecv(client);
                    if(strcmp(buffer, "exit\n") == 0) break;
                    printf("%s\n", buffer);
                    //send(conn, buffer, len , 0);把数据回发给客户端
                }
                /*用户输入信息了,开始处理信息并发送*/
                if(FD_ISSET(0, &read_fds))
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

/*
 * recv thread function
 *
 */
int myrecv( CONNECTION & client)
{
    char logmsg[512] = "";
    char logHead[64] = "";
    sprintf(logHead, "%s:%d --> %s:%d ", client.clientIP, client.clientPort, client.serverIP, client.serverPort);
    Mylog mylog;
    int length = 0;
    MSGBODY recvMsg;
    int err = 0;
    while(true)
    {
        recvMsg.length = 0;
        memset(&recvMsg, 0, sizeof(recvMsg));
        // recv head, to get the length of msg
        length = recv(client.socket_fd, &recvMsg, MSGHEAD_LENGTH, 0);
        if(length == -1)     // recv error
        {
            err = errno;
            if(err != 11)   // data isnot ready when errno = 11, log other error
            {
                sprintf(logmsg, "ERROR: %s recv error: %d--%s",logHead, errno, strerror(errno) );
                mylog.logException(logmsg);
                if(err == 9)
                {
                    close(client.socket_fd);
                    client.status = 0;
                    mylog.logException("ERROR: recv exit.");
                    return 0;
                }
            }
            //sleep(1);
            //usleep(10000);                // 10ms
            length = 0;                   // set it back to 0
            continue;
        }
        else                              // recv success
        {
            if( length == 0 )
            {
                close(client.socket_fd);
                client.status = 0;
                sprintf(logmsg, "INFO: %s: The client exited. Recv thread exit.", logHead);
                mylog.logException(logmsg);
                return 0;
            }
        }
        // recv msg head to get length, then get msg by length
        if(0 != recvMsg.length)
        {
            printf("type = %d, recvLen = %d,\n", recvMsg.type, recvMsg.length);
            length = recv(client.socket_fd, recvMsg.msg, recvMsg.length, 0);
            if(length == -1)     // recv
            {
                err = errno;
                if(err != 11) // data isnot ready when errno = 11, log other error
                {
                    sprintf(logmsg, "ERROR: %s recv msg error: %d--%s",logHead, errno, strerror(errno) );
                    mylog.logException(logmsg);
                }
                if(err == 9)
                {
                    close(client.socket_fd);
                    client.status = 0;
                    mylog.logException("ERROR: recv exit.");
                    return 0;
                }
                //sleep(1);
                usleep(10000);  // 10ms
                length = 0;  // set it back to 0
                continue;
            }
            else                     // recv success
            {

            /*    int ret = 0;
                ret = msgCheck(&recvBuf);
                if(ret == 0)  // valid msg
                {
                //  printf("sizeof SendQueue: %lu, RecvQueue: %lu\n", mp_msgQueueSend->size(), mp_msgQueueRecv->size());

                }
                else
                {
                    mylog.logException("INFO: msg invalid.");
                }*/
                if( length == 0 )
                {
                    close(client.socket_fd);
                    client.status = 0;
                    sprintf(logmsg, "INFO: %s: The client exited. Recv thread exit.", logHead);
                    mylog.logException(logmsg);
                    return 0;
                }else
                {
                    logMsg(&recvMsg, logHead, 1);
                }

            }// end if,  recv finished
        }

    }
    return 0;
}

int logMsg(const MSGBODY *pMsg, const char *logHead, int isRecv)
{
    Mylog mylog;
    if(pMsg == NULL)
        return -1;
    char logmsg[256] = "";
    char direction[32] = "";
    if(1 == isRecv)
        sprintf(direction, "received");
    else
        sprintf(direction, "send");
    if(2 == pMsg->type)             //  hex
    {
        try
        {
            char *p_hexLog = new char[pMsg->length*3 + 128];    // include the logHead
            memset(p_hexLog, 0, pMsg->length*3 + 128);
            sprintf(p_hexLog, "INFO: %s %s: ", logHead, direction);
            int len = strlen(p_hexLog);
            for(int i=0; i<pMsg->length; i++)
                sprintf(p_hexLog+len+3*i, "%02x ", (unsigned char)pMsg->msg[i]);
            mylog.logException(p_hexLog);
            delete[] p_hexLog;
        }catch(bad_alloc& bad)
        {
            sprintf(logmsg,"ERROR: Failed to alloc mem when log hex: %s", bad.what());
            mylog.logException(logmsg);
        }
    }
    else if(1 == pMsg->type)
    {
        char logmsg[pMsg->length + 128];
        memset(logmsg, 0, pMsg->length + 128);
        sprintf(logmsg, "INFO: %s %s: %s", logHead, direction, pMsg->msg);
        mylog.logException(logmsg);
    }
    else if(0 == pMsg->type)   // int
    {
        char logmsg[pMsg->length + 128];
        memset(logmsg, 0, pMsg->length + 128);
        sprintf(logmsg, "INFO: %s %s: %d", logHead, direction, *(int *)pMsg->msg);
        mylog.logException(logmsg);
    }
    return 0;
}
