/*
 * testHashMap.cpp
 *
 *  Created on: Dec 17, 2018
 *      Author: gyd
 */

#include <unordered_map>
using namespace std;
int test_unorderedMap()
{
    unordered_map<string, int> mymap;
    mymap.insert(pair<string, int>("Lisa", 28));
 //   mymap.insert(pair<string, int>("Anna", 23));
 //   mymap.insert(pair<string, int>("Nick", 18));
  //  mymap.insert(pair<string, int>("Nick", 18));
    std::unordered_map<string, int>::iterator mapiterator;
    for (mapiterator = mymap.begin(); mapiterator != mymap.end(); mapiterator++)
        printf("%s, %d \n", mapiterator->first.c_str(), mapiterator->second);
    printf("Num %lu/%lu,  used mem %lu", mymap.size(), mymap.max_size(), sizeof(mymap));
    return 0;
}


