/*
 * testMysqlclient.cpp
 *
 *  Created on: May 5, 2019
 *      Author: gyd
 */

#include "testMysqlclient.h"

testMysqlclient::testMysqlclient() {
    // TODO Auto-generated constructor stub

}

testMysqlclient::~testMysqlclient() {
    // TODO Auto-generated destructor stub
}

int testMysqlclient::mysqlconnect()
{
    MYSQL mysql;
    mysql_init(&mysql);
    /*使用mysql_real_connect连接server,其參数依次为
     * MYSQL句柄。serverIP地址,登录mysql的username，password，要连接的数据库等*/
    if(!mysql_real_connect(&mysql, "10.1.24.141", "root", "123456", "test", 0, NULL, 0))
       printf("Error connecting to Mysql!\n");
    else
       printf("Connected Mysql successful!\n");

    /*关闭连接*/
    mysql_close(&mysql);
    return 0;
}
