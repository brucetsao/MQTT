#include <ArduinoJson.h>  //Json 使用元件
//StaticJsonDocument<2000> json_doc;
//JsonArray temp = doc.to<JsonArray>();
//JsonArray humidp = doc.to<JsonArray>();
 // static JsonArray Temperature = json_doc.createNestedArray("Temperature");
 // static  JsonArray Humidity = json_doc.createNestedArray("Humidity");
//const int capacity = JSON_ARRAY_SIZE(30) + 30*JSON_OBJECT_SIZE(1);
//StaticJsonDocument<capacity> json_row1;
//StaticJsonDocument<capacity> json_row2;
int arraycount = 0 ;
#define  arrayamount 50
String Devname ;
double Temp[arrayamount] ;
double Humid[arrayamount] ;

char json_data[8000];
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

   // json_doc["Device"] ="1234567890AB" ;
  //  json_doc["Device"] ="1234567890AB" ;
    for (int i=0 ; i< arrayamount; i++)
    {
   //     Temperature.add(0);
    //    Humidity.add(0);     
    }

}


void setjsondata(String mm,double *t, double *h)
{
    StaticJsonDocument<2000> json_doc;
  json_doc["Device"] = mm ;
  JsonArray Temperature = json_doc.createNestedArray("Temperature");
  JsonArray Humidity = json_doc.createNestedArray("Humidity");
  json_doc["Device"] = mm;
  for(int i=0;i<arrayamount;i++)
  {
    Temperature.add(*(t+i)) ;
    Humidity.add(*(h+i)) ;
  }
 // json_doc["Temperature"] =  json_rowT;
 // json_doc["Humidity"] =  json_rowH;
   serializeJson(json_doc, json_data); 

}

void appendjsondata(Sensor dd, double *t,double *h, int pp)
{
  *(t+pp) = dd.Temperature;
  *(h+pp) = dd.Humidity;
}
