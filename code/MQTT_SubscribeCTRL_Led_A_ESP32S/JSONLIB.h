#include <ArduinoJson.h>  //Json 使用元件
StaticJsonDocument<1000> json_doc;
StaticJsonDocument<100> json_rowdata;

unsigned char json_data[5000];
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


void setjsondata()
{
   serializeJson(json_row, json_data); 

}

void appendjsondata(String mm,double d1, double d2)
{

  json_rowdata["Device"] = mm;
  json_rowdata["Temperature"] = d1;
  json_rowdata["Humidity"] = d2;
  json_row.add(json_rowdata) ;
}
