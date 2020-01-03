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






