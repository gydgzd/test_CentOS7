/*
 * RawSocket.cpp
 *
 *  Created on: Jan 3, 2020
 *      Author: gyd
 */

#include "RawSocket.h"

RawSocket::RawSocket() {
    // TODO Auto-generated constructor stub
    m_rawSocket = 0;
    memset(m_buff, 0, 4096);
    initSocket();
}

RawSocket::~RawSocket() {
    // TODO Auto-generated destructor stub
}

int RawSocket::initSocket()
{

    if((m_rawSocket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
    {
        printf("socket create error: %d - %s\n", errno, strerror(errno));
    }


    return 0;
}
int n = 0;
const char hello[] = "This is my ping.";
//计算校验和
u16 checksum(u8 *buf,int len)
{
    u32 sum=0;
    u16 *cbuf;
    cbuf=(u16 *)buf;
    while(len>1)
    {
        sum+=*cbuf++;
        len-=2;
    }
    if(len)
        sum+=*(u8 *)cbuf;
    sum=(sum>>16)+(sum & 0xffff);
    sum+=(sum>>16);
    return ~sum;
}
int RawSocket::sendPkt()
{
    struct iphdr *ip_hdr;   //iphdr为IP头部结构体
    struct icmphdr *icmp_hdr;   //icmphdr为ICMP头部结构体
    int len;
    int i;
    icmp_hdr=(struct icmphdr *)(m_buff);  //字符串指针
    icmp_hdr->type=ICMP_ECHO;    //初始化ICMP消息类型type
    icmp_hdr->code=0;    //初始化消息代码code
    icmp_hdr->un.echo.id=1;   //把进程标识码初始给icmp_id
    icmp_hdr->un.echo.sequence=n++;  //发送的ICMP消息序号赋值给icmp序号
//    gettimeofday((struct timeval *)icmp_hdr->icmp_timestamp,NULL); // 获取当前时间
    icmp_hdr->icmp_timestamp[0] = 0x5E11C643;

    memcpy(icmp_hdr->data, hello, strlen(hello));

    struct sockaddr_in dst;
    memset(&dst, 0, sizeof(dst));
    dst.sin_addr.s_addr =  inet_addr("10.1.24.128");
    dst.sin_family = PF_INET;
    dst.sin_port = ntohs(0);

    len=ICMP_HSIZE+strlen(hello);
    icmp_hdr->checksum=0;    //初始化
    icmp_hdr->checksum=checksum((u8 *)icmp_hdr,len);  //计算校验和
    for(i=0; i < len; i++)
    {
        printf("%02X", (unsigned char)m_buff[i]);
        if((i+1)%8 == 0)
            printf("  ");
    }
    printf("\n");
  //  printf("The send pack checksum is:0x%x\n",icmp_hdr->checksum);
    sendto(m_rawSocket, m_buff, len, 0, (struct sockaddr *)&dst, sizeof (dst)); //经socket传送数据

    return 0;
}




