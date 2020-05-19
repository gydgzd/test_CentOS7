/*
 * testRapidJson.cpp
 *
 *  Created on: May 13, 2019
 *      Author: gyd
 */

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <sys/stat.h>           // mkdir stat
#include <iostream>
#include <memory>               // shared_ptr
using namespace std;
using namespace rapidjson;

int getFileSize(const char * filename)
{
    struct stat buf;
    long size = 0;
    if((stat(filename,&buf)!=-1))
    {
        size = buf.st_size;
    }
    else
    {
        printf("getFileSize failed: %d - %s\n" , errno, strerror(errno));;
        return -1;
    }
    return size;
}
void CloseFile(FILE *fp)
{
    if(fp == nullptr)
        return;
    fclose(fp);
    fp = nullptr;
    printf("file closed\n");
}
int testRapidJson() {
    char filename[256] = "conf.json";
    long fileSize = getFileSize(filename);

    FILE *fp = fopen(filename , "r");
    shared_ptr<FILE> fileptr(fp, CloseFile);
    if(fp == NULL) {
        perror("打开文件时发生错误");
        return(-1);
    }

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);

    char *json = new char[fileSize + 64];
    shared_ptr<char> buffer(json, std::default_delete<char[]>());
    fseek(fp, 0, SEEK_SET);
    fread(json, sizeof(char), fileSize + 1, fp);

    // Parse a JSON string into DOM.
 //   const char* json = "{  \n\"date\":\"2019-05-29 14:45:36:835699\",\n\"uvpa\":\"10.1.24.213(213抓包)\",\n\"cmd\" :\"8f85\",\n\"subcmd\" :\"6001\",\n\"srcMac\" :\"B8EA6A06B9B2\",\n\"dstMac\" :\"7C11CD000000\",\n\"srcName\" :\"启明3C\",\n\"dstName\" :\"slave\" \n},\n{ \n\"date\":\"2019-05-29 14:45:36:835699\",\n\"uvpa\":\"10.1.24.213(213抓包)\",\n\"cmd\" :\"8f85\",\n\"subcmd\" :\"6001\",\n\"srcMac\" :\"7C11CD000001\",\n\"dstMac\" :\"7C11CD00C001\",\n\"srcName\" :\"slave\",\n\"dstName\" :\"主服务器\" \n},\n{ \n\"date\":\"2019-05-29 14:45:36:835921\",\n\"uvpa\":\"10.1.24.235(235抓包)\",\n\"cmd\" :\"8f85\",\n\"subcmd\" :\"6001\",\n\"srcMac\" :\"7C11CD00C001\",\n\"dstMac\" :\"7C11CD00A001\",\n\"srcName\" :\"主服务器\",\n\"dstName\" :\"从服务器1\" \n},\n{ \n\"date\":\"2019-05-29 14:45:36:835921\",\n\"uvpa\":\"10.1.24.235(235抓包)\",\n\"cmd\" :\"8785\",\n\"subcmd\" :\"6001\",\n\"srcMac\" :\"7C11CD00A000\",\n\"dstMac\" :\"60F2EF02A690\",\n\"srcName\" :\"从服务器1\",\n\"dstName\" :\"极光18002\" \n} \n";
/*    const char* json = "{\"path\":[{ "
            "\"date\":\"2019-05-29 14:45:36:835699\","
            "\"uvpa\":\"10.1.24.213(213抓包)\","
            "\"cmd\" :\"8f85\","
            "\"subcmd\" :\"6001\","
            "\"srcMac\" :\"B8EA6A06B9B2\","
            "\"dstMac\" :\"7C11CD000000\","
            "\"srcName\" :\"启明3C\","
            "\"dstName\" :\"slave\" },{ "
            "\"date\":\"2019-05-29 14:45:36:835699\","
            "\"uvpa\":\"10.1.24.213(213抓包)\","
            "\"cmd\" :\"8f85\","
            "\"subcmd\" :\"6001\","
            "\"srcMac\" :\"7C11CD000001\","
            "\"dstMac\" :\"7C11CD00C001\","
            "\"srcName\" :\"slave\","
            "\"dstName\" :\"主服务器\" }] }";
*/
 //   const char* json = "{\"funcName\":\"get_probe_version\",\"param\":{\"uuid\":\"servers_9fc1a187-f420-4320-a4d3-ab332b4ff906\",\"userId\":\"99f371f98c5342c3a5477bddae1b45ad\"}}";
    Document doc;
    Value::ConstMemberIterator iter;
    if (doc.Parse(json).HasParseError())
    {
        cout << "parse error" << endl;
        return 1;
    }
    // get string of an object
    //  Value &path_resolve = doc["path"];

    iter = doc.FindMember("Device");           // can be faster
    if (iter != doc.MemberEnd())
    {
        Value &path_resolve = doc["Device"];
        if(path_resolve.IsArray() == true )
        {
            printf("Is Array\n");
        }
        if(doc["Device"].IsArray() == true )   // Value path_resolve = doc["path"];  may have a problem
        {
            printf("Is Array\n");
        }

        StringBuffer buffer1;
        //    Writer<StringBuffer> writer(buffer);    //writer filtered the blanks
        //    doc.Accept(writer);
            PrettyWriter<StringBuffer> pretty_writer1(buffer1);
            path_resolve.Accept(pretty_writer1);
     //       std::cout << buffer1.GetString() << std::endl;


        if(path_resolve.IsArray() == true )
        {
            for (SizeType i = 0; i < path_resolve.Size(); i++) // 使用 SizeType 而不是 size_t
                printf("path[%d] : %s %s %s-%s %s %s\n", i,
                        path_resolve[i].FindMember("date")->value.GetString(),
                        path_resolve[i].FindMember("uvpa")->value.GetString(),
                        path_resolve[i].FindMember("cmd")->value.GetString(),
                        path_resolve[i].FindMember("subcmd")->value.GetString(),
                        path_resolve[i].FindMember("srcMac")->value.GetString(),
                        path_resolve[i].FindMember("dstMac")->value.GetString()
                        );
        }
    }

    // Modify it by DOM.
    if(doc.HasMember("VirtualMac") == true )
    {
        cout << doc["VirtualMac"].GetString() << " len: " << doc["VirtualMac"].GetStringLength() << " type: " << doc["VirtualMac"].GetType() << endl;

        Value& stat = doc["TUN_NAME"];
        stat.SetDouble(9.09);
    }
    //
    iter = doc.FindMember("TUN_IP");
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
    else
        cout << "No TUN_IP" << endl;
    //
    iter = doc.FindMember("TUN_NETMASK");
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
    else
        cout << "No TUN_NETMASK" << endl;
    //
    iter = doc.FindMember("TUN_GATEWAY");
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
    else
        cout << "No TUN_GATEWAY" << endl;
    //
    iter = doc.FindMember("TUN_MTU");
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
    else
        cout << "No TUN_MTU" << endl;
    // add member
    Document::AllocatorType& allocator = doc.GetAllocator();

    Value object1(kObjectType);
    object1.AddMember("date",   "2019-05-13 08:22:21",allocator);
    object1.AddMember("cmd",    "8f85",                allocator);
    object1.AddMember("subcmd", "8785",                allocator);
    object1.AddMember("src",    "ff01",                allocator);
    object1.AddMember("dst",    "0002",                allocator);

    Value object2(kObjectType);
    object2.AddMember("date",   "2019-05-13 08:22:22",allocator);
    object2.AddMember("cmd",    "8f85",                allocator);
    object2.AddMember("subcmd", "8785",                allocator);
    object2.AddMember("src",    "0002",                allocator);
    object2.AddMember("dst",    "ef21",                allocator);
    // add array
    Value& path = doc["path"];   // This time we uses non-const reference.
    path.SetArray();
    path.PushBack(object1, allocator);
    path.PushBack(object2, allocator);
    // Fluent API
    //path.PushBack(object1, allocator).PushBack(object2, allocator);
/*
    iter = doc.FindMember("state");           // can be faster
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
*/
 //   Value& s = doc["bits"];
 //   s.SetInt(s.GetInt() + 1);

    // Stringify the DOM
    StringBuffer buffer1;
//    Writer<StringBuffer> writer(buffer);    //write filtered the blanks
//    doc.Accept(writer);
    PrettyWriter<StringBuffer> pretty_writer(buffer1);
    doc.Accept(pretty_writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer1.GetString() << std::endl;

    // get array
    iter = doc.FindMember("path");           // can be faster
    if (iter != doc.MemberEnd())
    {
        Value& path_resolve = doc["path"];
        if(path_resolve.IsArray() == true )
        {
            for (SizeType i = 0; i < path_resolve.Size(); i++) // 使用 SizeType 而不是 size_t
                printf("path[%d] : %s %s-%s %s %s\n", i, path_resolve[i].FindMember("date")->value.GetString(),
                        path_resolve[i].FindMember("cmd")->value.GetString(),
                        path_resolve[i].FindMember("subcmd")->value.GetString(),
                        path_resolve[i].FindMember("src")->value.GetString(),
                        path_resolve[i].FindMember("dst")->value.GetString() );
        }
    }
    return 0;
}


