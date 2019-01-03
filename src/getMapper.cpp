/*
 * getMapper.cpp
 * This function get map relations from physical volume to virtual group(lvm)
 *  Created on: Jul 22, 2016
 *      Author: Gyd
 */
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
int getMapper()
{
	//get the file include the relation, mapper is the name
	system("ls -l /dev/mapper > mapper");

	//parse the file
	ifstream readf;
	readf.open("mapper");
    string line = "";
	string filetype = "";
	string filename="";
	string linkname="";
	string tmp[8] ;
    //stat_new.stat_old;
	while (getline (readf, line) ) // read lines
	{
	    stringstream ss(line);
	    if(line.substr(0,1)!="l")   //l means a link
	    	continue;
	   // cout<<"line is :"<<line<<endl;
	    for(int i=0; i<8; i++)          // there are 8 fields before the filename
	    {
	    	ss>>tmp[i];
	    }
	    ss>>filename;
	    ss>>tmp[0];
	    ss>>linkname;


	    cout<<filename<<" "<<linkname<<endl;

	}

	readf.close();
	return 0;
}



