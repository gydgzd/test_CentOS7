/*
 * RawSocket.h
 *
 *  Created on: Jan 3, 2020
 *      Author: gyd
 */

#ifndef RAWSOCKET_H_
#define RAWSOCKET_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <signal.h>
#include <sys/time.h>
#include <netdb.h>              // gethostbyname
#include <linux/if_ether.h>
#include <linux/if_packet.h>
//#include <linux/icmp.h>

#define ICMP_HSIZE sizeof(struct icmphdr)
#define ICMP_ECHOREPLY 0 //Echo应答
#define ICMP_ECHO      8 //Echo请求
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
struct icmphdr
{
    u8 type;
    u8 code;
    u16 checksum;
    union
    {
        struct
        {
            u16 id;
            u16 sequence;
        }echo;
        u32 gateway;
        struct
        {
            u16 unused;
            u16 mtu;
        }frag; //pmtu发现
    }un;
    u32  icmp_timestamp[2];//时间戳
    //ICMP数据占位符
    u8 data[0];
#define icmp_id un.echo.id
#define icmp_seq un.echo.sequence
};
class RawSocket {
public:
    RawSocket();
    virtual ~RawSocket();

    int initSocket();
    int sendPkt();

private:
    int m_rawSocket;
    char m_buff[4096];


};

#endif /* RAWSOCKET_H_ */
