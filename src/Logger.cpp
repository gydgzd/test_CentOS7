/*
 * Logger.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: gyd
 */
#include "Logger.h"

Logger::Logger():mExit(false)
{
	//Start background thread.
	mThread = thread(&Logger::processEntries, this);
}

Logger::~Logger()
{
	{
		unique_lock<mutex> lock(mMutex);
		//shut down the thread by setting mExit
		mExit = true;
		mCondVar.notify_all();
	}
	mThread.join();
}

void Logger::log(const string& entry)
{
	//Lock mutex and add entry to the queue.
	unique_lock<mutex> lock(mMutex);
	mQueue.push(entry);
	//notify condition variable to wake up threads
	mCondVar.notify_all();
}

void Logger::processEntries()
{
	//open log file
	ofstream ofs("SysMon_log.txt");
	if(ofs.fail())
	{
		cerr<<"Failed to open log file."<<endl;
		return;
	}
	//start process loop
	unique_lock<mutex> lock(mMutex);
	while(true)
	{
		//Only wait for notification if we don't have to exit
		if(!mExit)
		{
			mCondVar.wait(lock);  //wait for a notification
		}
		//Condition variable is not notified, so something might be in the queue.
		lock.unlock();
		while(true)
		{
			lock.lock();
			if(mQueue.empty() )
			{
				break;
			}
			else
			{
				ofs<<mQueue.front()<<endl;
				mQueue.pop();
			}
			lock.unlock();
		}
		if(mExit)
			break;
	}
	ofs.close();
}

////test
void logSomeMessages(int id, Logger& logger)
{
	for(int i=0; i<10; i++)
	{
		stringstream ss;
		ss<<"Log entry"<<i;
		ss<<" from thread "<<id;
		logger.log(ss.str() );
	}

}
int test()
{
	Logger logger;
	vector<thread> threads;
	for(int i=0 ; i<3; i++)
	{
		threads.emplace_back(logSomeMessages, i ,ref(logger));

	}
	//wait for all threads to finish
	for(auto& t:threads)
	{
		t.join();
	}
	return 0;
}
