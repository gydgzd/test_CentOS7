/*
 * testRapidJson.cpp
 *
 *  Created on: May 13, 2019
 *      Author: gyd
 */

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

using namespace std;
using namespace rapidjson;

int testRapidJson() {
    // 1. Parse a JSON string into DOM.
    const char* json = "{ \"meetingID\": \"%s\",\n"
            "\"meetingName\": \"2312c313x4aw2\", \n"
            "\"date\": \"2019-05-13 08:22:21\",\n"
            "\"state\": \"0\",\n"
            "\"dstDevNo\": \"%s\",\n"
            "\"path\": \"%s\",\n"
            "\"deviceName\": \"%s\",\n"
            "\"deviceInfo\": \"\",\n"
            "\"bits\": 16,\n"
            "\"source\": \"信令分析\"}";

    Document doc;
    if (doc.Parse(json).HasParseError())
    {
        cout << "parse error" << endl;
        return 1;
    }
    else
        cout << "parse successful" << endl;

    // 2. Modify it by DOM.
    if(doc.HasMember("state") == true )
        cout << doc["state"].GetString() << " len: " << doc["state"].GetStringLength() << " type: " << doc["state"].GetType() << endl;

    Value::ConstMemberIterator iter = doc.FindMember("type");
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
    else
        cout << "No type" << endl;

    Value& path = doc["path"];   // This time we uses non-const reference.
    Document::AllocatorType& allocator = doc.GetAllocator();
/*    for (int i = 0; i <= 2; i++)
        path.PushBack(i, allocator);   // May look a bit strange, allocator is needed for potentially realloc. We normally uses the document's.
*/
    Value path_array(kArrayType);

       Value int_object(kObjectType);
       int_object.SetString("srcMAC");
       path_array.PushBack(int_object,allocator);

    doc.AddMember("path",path_array,allocator);





    // Fluent API
//    path.PushBack("8f85", allocator).PushBack("8785", allocator);
/*
    iter = doc.FindMember("state");
    if (iter != doc.MemberEnd())
        printf("%s\n", iter->value.GetString());
*/
    Value& s = doc["bits"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0;
}


