#include <ArduinoJson.h>  //Json 使用元件
StaticJsonDocument<3000> json_doc;
StaticJsonDocument<100> json_rowdata;

const int capacity = JSON_ARRAY_SIZE(10) + 10*JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> json_row;
int arraycount = 0 ;
#define  arrayamount 10

char json_data[5000];
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


void setjsondata(String mm)
{
  json_doc["Device"] = mm;
  json_doc["Data"] =  json_row;
   serializeJson(json_doc, json_data); 

}

void appendjsondata( double d1, double d2)
{
;
  json_rowdata["Temperature"] = d1;
  json_rowdata["Humidity"] = d2;
  json_row.add(json_rowdata) ;
}
