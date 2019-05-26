/*
 * testAmqpcpp.cpp
 *
 *  Created on: May 22, 2019
 *      Author: gyd
 */

#include "AMQPcpp.h"

using namespace std;

int testAmqpcpp()
{
    try {
//      AMQP amqp;
//      AMQP amqp(AMQPDEBUG);

        AMQP amqp("topo:topo@10.1.24.141:5672");     // all connect string

        AMQPExchange * ex = amqp.createExchange("e");
        ex->Declare("e", "fanout");

        AMQPQueue * qu2 = amqp.createQueue("q2");
        qu2->Declare();
        qu2->Bind( "e", "");

        string ss = "message 1 ";
        /* test very long message
        ss = ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
*/

        ex->setHeader("Delivery-mode", 2);
        ex->setHeader("Content-type", "text/text");
        ex->setHeader("Content-encoding", "UTF-8");

    //    ex->Publish(  ss , ""); // publish very long message

    //    ex->Publish(  "message 2 " , "");
    ///    ex->Publish(  "message 3 " , "");   // it takes long time



    } catch (AMQPException &e)
    {
        std::cout << e.getMessage() << std::endl;
    }

    return 0;

}


