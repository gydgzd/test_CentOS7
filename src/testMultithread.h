/*
 * testMultithread.h
 *
 *  Created on: Feb 1, 2019
 *      Author: gyd
 */

#ifndef TESTMULTITHREAD_H_
#define TESTMULTITHREAD_H_


#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
using namespace std;

extern int g_num;
extern std::mutex g_num_mutex;
class Mycounter
{
public:
    Mycounter() {};
    ~Mycounter()
    {
        printf("~Mycounter()\n");
    };
    static int count(int n, int id)
    {
        for (int i = 0; i < n; i++)
        {
            std::lock_guard<std::mutex> guard(g_num_mutex);   // 加锁，保证下面语句原子执行
            cout << id << " " << g_num++ << endl;
        }
        sleep(2);
        return 0;
    }
    int mtdCount()
    {
        thread th1{ &Mycounter::count, 10, 1 };
        thread th2{ &Mycounter::count, 10, 2 };
        thread th3{ &Mycounter::count, 10, 3 };
        th1.join();
        th2.join();
        th3.join();
        return 0;
    }
    void counter(int iterations, int id)
    {
        for (int i = 0; i < iterations; i++)
        {
            printf("%d %d\n", i, id);
        }
    }
    void testThread()
    {
        Mycounter mc1;
        mc1.mtdCount();

        std::thread th{ &Mycounter::counter, &mc1,10, 1 };
        th.detach();
        sleep(10);
    }
};


#endif /* TESTMULTITHREAD_H_ */
