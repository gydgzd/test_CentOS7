/*
 * myTunTap.cpp
 *
 *  Created on: Jan 7, 2020
 *      Author: root
 *      # ip link set proxy_tun up
 *      # ip addr add 10.0.0.1/24 dev proxy_tun
 */

#include "myTunTap.h"

myTunTap::myTunTap() {
    // TODO Auto-generated constructor stub
    m_fd = 0;
    strcpy(m_devName, "proxy_tun");
    m_fd = dev_alloc(m_devName, IFF_TUN | IFF_NO_PI);
    if(m_fd < 0){
        perror("Allocating interface");
        exit(1);
    }
}

myTunTap::~myTunTap() {
    // TODO Auto-generated destructor stub
}


/* Arguments taken by the function:
 * char *dev: the name of an interface (or '\0'). MUST have enough
 *             space to hold the interface name if '\0' is passed
 * int flags: interface flags (eg, IFF_TUN etc.)
 */
int myTunTap::dev_alloc(char *dev, int flags)
{
    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";

    /* open the clone device */
    if( (fd = open(clonedev, O_RDWR)) < 0 ) {
        return fd;
     }
     // preparation of the struct ifr, of type "struct ifreq"
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags;   // IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI

    if (*dev) {
        // if a device name was specified, put it in the structure; otherwise,
        // the kernel will try to allocate the "next" device of the specified type
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    /* try to create the device */
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        close(fd);
        return err;
    }

//    strcpy(dev, ifr.ifr_name);
    system("ip link set proxy_tun up");
    system("ip addr add 10.0.0.1/24 dev proxy_tun");
    return fd;
}

int myTunTap::dev_read()
{

    int nread = 0;
    unsigned char buffer[4096];
    /* Now read data coming from the kernel */
    while(1)
    {
        memset(buffer, 0, 4096);
        /* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
        nread = read(m_fd, buffer,sizeof(buffer));
        if(nread < 0)
        {
            perror("Reading from interface");
            close(m_fd);
            exit(1);
        }

        /* Do whatever with the data */
        printf("Read %d bytes from device %s\n", nread, m_devName);
        for(int n = 0; n < nread; n++)
        {
            printf("%02x", buffer[n]);
            if( (n+1)%8 == 0)
                printf("  ");
        }
        printf("\n");
    }
    return 0;
}

int myTunTap::dev_write()
{
    const char hello[] = "This is my ping.";
    unsigned char buffer[4096];
    struct iphdr *ip_hdr;   //iphdr为IP头部结构体
    struct icmphdr *icmp_hdr;   //icmphdr为ICMP头部结构体
    int len;
    int i;
    ip_hdr = (struct iphdr *)(buffer);
    icmp_hdr=(struct icmphdr *)(buffer + sizeof(struct iphdr));  //字符串指针
    // Fabricate the IP header or we can use the
    // standard header structures but assign our own values.
    ip_hdr->ihl = 5;
    ip_hdr->version = 4;//报头长度，4*32=128bit=16B
    ip_hdr->tos = 0; // 服务类型
    ip_hdr->tot_len = ((sizeof(struct iphdr) + sizeof(struct icmphdr) + strlen(hello)));
    //ip->id = htons(54321);//可以不写
    ip_hdr->ttl = 64; // hops生存周期
    ip_hdr->protocol = IPPROTO_ICMP; // UDP
    ip_hdr->check = 0;
    // Source IP address, can use spoofed address here!!!
    ip_hdr->saddr = inet_addr("10.1.24.25");
    // The destination IP address
    ip_hdr->daddr = inet_addr("10.1.24.52");

    icmp_hdr->type=8;    //初始化ICMP消息类型type
    icmp_hdr->code=0;    //初始化消息代码code
    icmp_hdr->un.echo.id=1;   //把进程标识码初始给icmp_id
    icmp_hdr->un.echo.sequence=1;  //发送的ICMP消息序号赋值给icmp序号
//    gettimeofday((struct timeval *)icmp_hdr->icmp_timestamp,NULL); // 获取当前时间
    icmp_hdr->icmp_timestamp[0] = 0x5E11C643;

    memcpy(icmp_hdr->data, hello, strlen(hello));

    len=20 + ICMP_HSIZE+strlen(hello);
    icmp_hdr->checksum=0;    //初始化
    icmp_hdr->checksum=checksum((unsigned char *)icmp_hdr,len);  //计算校验和
    for(i=0; i < len; i++)
    {
        printf("%02X", (unsigned char)buffer[i]);
        if((i+1)%8 == 0)
            printf("  ");
    }
    printf("\n");
  //  printf("The send pack checksum is:0x%x\n",icmp_hdr->checksum);
    int nwrite = 0;
    if((nwrite=write(m_fd, buffer, len)) < 0)
    {
        perror("Writing data");
        return -1;
    }
    return 0;
}

