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



class RawSocket {
public:
    RawSocket();
    virtual ~RawSocket();

    int initSocket();


private:
    int m_rawSocket;



};

#endif /* RAWSOCKET_H_ */
