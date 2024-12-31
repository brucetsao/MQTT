#include <ArduinoJson.h>  //Json 使用元件
int arraycount = 0 ;    //目前溫溼度第n筆 計數器
#define  arrayamount 50   //一次丟json的資料量
String Devname ;    //資料收集器的MAC Address
double Temp[arrayamount] ;    //溫度陣列
double Humid[arrayamount] ;   //濕度陣列


Deserialization char json_data[8000];   //預留json資料儲存記憶體rror json_error;    //json錯誤代碼




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
  for(int i=0;i<arrayamount;i++)    //把50個變數都填到json array變數
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
