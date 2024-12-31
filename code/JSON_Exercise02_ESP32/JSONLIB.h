#include <ArduinoJson.h>  //Json 使用元件
StaticJsonDocument<500> json_doc;
char json_data[1000];
DeserializationError json_error;



/*

{
  "Device": "E89F6DE8F3BC",
  "Temperature": 24,
  "Humidity": 77
}

 */

void initjson()
{
    
}


void setjsondate(String mm, double d1, double d2)
{
  json_doc["Device"] = mm;
  json_doc["Temperature"] =  d1;
  json_doc["Humidity"] = d2;
  serializeJson(json_doc, json_data);
}
