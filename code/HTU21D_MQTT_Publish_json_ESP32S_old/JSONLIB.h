#include <ArduinoJson.h>  //Json 使用元件
StaticJsonDocument<2000> json_doc;
StaticJsonDocument<100> json_rowdata;

const int capacity = JSON_ARRAY_SIZE(30) + 30*JSON_OBJECT_SIZE(1);
StaticJsonDocument<capacity> json_row1;
StaticJsonDocument<capacity> json_row2;
int arraycount = 0 ;
#define  arrayamount 20

char json_data[2500];
DeserializationError json_error;
double Arraytemp[30] ;
double Arrayhumid[30] ;



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
  json_doc["Temperature"] =  Arraytemp;
  json_doc["Humidity"] =  Arrayhumid;
   serializeJson(json_doc, json_data); 

}

void appendjsondata(int cc, Sensor dd)
{
;
  Arraytemp[cc] = dd.Temperature;
  Arrayhumid[cc] = dd.Humidity;;
  //json_row.add(json_rowdata) ;
}
