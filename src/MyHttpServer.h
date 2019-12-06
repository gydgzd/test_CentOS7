/*
 * MyHttpServer.h
 *
 *  Created on: Jun 12, 2019
 *      Author: gyd
 */

#ifndef MYHTTPSERVER_H_
#define MYHTTPSERVER_H_


#include "mongoose.h"

#include <map>
#include <string>
#include <iostream>
#include <functional>

//namespace MyClass {

class MyHttpServer {
public:
    MyHttpServer();
    virtual ~MyHttpServer();

    int testHttp();
    int testTcpServer();
};

//} /* namespace MyClass */

#endif /* MYHTTPSERVER_H_ */
