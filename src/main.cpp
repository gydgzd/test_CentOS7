//============================================================================
// Name        : test.cpp
// Author      : gyd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <time.h>
#include <unistd.h>
using namespace std;

#include "myProgram.h"
#include "zmq_wrapper.h"
#include "zlib.h"
#include "zconf.h"
extern int my_zmq_server ();
extern int my_zmq_client ();
extern int testSQLite(int argc, char **argv);
string gzip_uncompress(const char *src, size_t length);
extern char * strReturn(void);
extern char* create_monitor(void);

extern int parse(const char * const str);
extern char* createJSON(void);
extern int utf82gbk(char *gbkStr, const char *srcStr, int maxGbkStrlen);
extern int testGets();
extern int test_unorderedMap();
extern int testMap();
extern int socket_server();

struct test
{
    int a;
    char b;
    int c;
};
int main(int argc, char ** argv)
{
 //   socket_server();
    testMap();
	test_unorderedMap();
/*	testGets();
	char c = 0xff;
	char d = 0xfe;
	if( 0 == memcmp(&c,&d, 1))
		cout<< "true" << endl;
	cout<< "false" <<endl;
	test t1;
	t1.a = 9;
	t1.b = 'a';
	t1.c = 12;
	printf("%x\n", &t1.a);
	printf("%64x, %d\n", t1, sizeof(t1));
	*/
//	char * str = create_monitor();
//	str = strReturn();
//	cout << str << endl;

//	testSQLite(argc, argv);
/*
	map<int, string> mtest;
    mtest[1].assign("name");
    if (0 == strcmp(argv[1], "s"))  // return 0 if equal
    	my_zmq_server ();
    else if (0 == strcmp(argv[1], "c"))
    	my_zmq_client ();
    else
    	cout << "param error!" << endl;
*/
/**/
	cZmqWrapper zw;
	zw.client("tcp://10.1.24.63:14200");
	char *buf = new char[512];
	while(true)
	{
		int rc=zw.recv(buf, 0x10000);
		if(rc<1) continue;
		else
		{

		//	string ss = gzip_uncompress(buf + 8, (unsigned long)rc);  // there is a t_live_head here,which is 8 bytes
		//	printf("%s\n", ss.c_str());
		//	printf("%s\n", buf);
		}
		buf = createJSON();
		cout << buf << endl;
	}
	delete[] buf;

	/*
	getVolum("/");
	getVolum("/boot");
	getMapper();
	searchPartTable("/dev/sda1");
	searchPartTable("/dev/mapper/centos-root");
	cout<<"├"<<endl;
	*/
/*	try
	{
		int x=3;
		cout<<x<<endl;
		throw(x);
	}
	catch(int)
	{
		cout<<"catch"<<endl;
	}
	char sa[5]="aa",sb[6]="bb";
	scanf("%s%s", sa, sb);
	cout<<sa<<" "<<sb<<endl;
	*/
/*
	vector<int> va;
	for (int i = 0; i < 16; i++)
	{
		cout << "size:" << va.size() << " Capacity:" << va.capacity() << endl;
		va.push_back(1);
	}
*/
	return 0;
}

