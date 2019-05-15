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
#include <iostream>

using namespace std;
using namespace rapidjson;

int testRapidJson() {
    // Parse a JSON string into DOM.
    const char* json = "{ \"meetingID\": \"a8dh31lbjsbakueh2oadw2\",\n"
            "\"meetingName\": \"信令测试\", \n"
            "\"date\": \"2019-05-13 08:22:21\",\n"
            "\"state\": \"0\",\n"
            "\"dstDevNo\": \"10086\",\n"
            "\"path\": \"%s\",\n"
            "\"bits\": 16}";

    Document doc;
    if (doc.Parse(json).HasParseError())
    {
        cout << "parse error" << endl;
        return 1;
    }

    // Modify it by DOM.
    if(doc.HasMember("state") == true )
    {
        cout << doc["state"].GetString() << " len: " << doc["state"].GetStringLength() << " type: " << doc["state"].GetType() << endl;

        Value& stat = doc["state"];
        stat.SetDouble(9.09);
    }

    Value::ConstMemberIterator iter = doc.FindMember("type");
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
    else
        cout << "No type" << endl;
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
    iter = doc.FindMember("state");      // can be faster
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
*/
    Value& s = doc["bits"];
    s.SetInt(s.GetInt() + 1);

    // Stringify the DOM
    StringBuffer buffer;
//    Writer<StringBuffer> writer(buffer);    //write filtered the blanks
//    doc.Accept(writer);
    PrettyWriter<StringBuffer> pretty_writer(buffer);
    doc.Accept(pretty_writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    // get array
    Value &path_resolve = doc["path"];
    if(path_resolve.IsArray() == true )
    {
        for (SizeType i = 0; i < path_resolve.Size(); i++) // 使用 SizeType 而不是 size_t
            printf("path[%d] : %s %s-%s %s %s\n", i, path_resolve[i].FindMember("date")->value.GetString(),
                    path_resolve[i].FindMember("cmd")->value.GetString(),
                    path_resolve[i].FindMember("subcmd")->value.GetString(),
                    path_resolve[i].FindMember("src")->value.GetString(),
                    path_resolve[i].FindMember("dst")->value.GetString() );
    }

    return 0;
}


