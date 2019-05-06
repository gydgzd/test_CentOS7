/*
 * testMysqlclient.h
 *
 *  Created on: May 5, 2019
 *      Author: gyd
 */

#ifndef TESTMYSQLCLIENT_H_
#define TESTMYSQLCLIENT_H_

#include <stdio.h>
#include <mysql.h>



class testMysqlclient {
public:
    testMysqlclient();
    virtual ~testMysqlclient();

    int mysqlconnect();






};

#endif /* TESTMYSQLCLIENT_H_ */
