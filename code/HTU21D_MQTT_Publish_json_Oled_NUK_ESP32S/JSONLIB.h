/*
這段程式碼的註解說明了如何使用 ArduinoJson 庫，
將溫溼度資料存放到陣列並序列化成 JSON 格式。
 */
#include <ArduinoJson.h>  // Json 使用元件
int arraycount = 0;    // 目前溫溼度第 n 筆計數器
#define arrayamount 10   // 一次要傳送的 JSON 資料量
String Devname;    // 資料收集器的 MAC Address
double Temp[arrayamount];    // 儲存溫度的陣列
double Humid[arrayamount];   // 儲存濕度的陣列
double tempsum;
double humidsum;


char json_data[1000];   // 預留 JSON 資料儲存記憶體
DeserializationError json_error;    // JSON 反序列化錯誤代碼

/*
{
  "Device": "E89F6DE8F3BC",
  "Temperature": 24,
  "Humidity": 77
}
*/

void initjson()   // 初始化 JSON 資料
{
   // json_doc["Device"] ="1234567890AB" ;
//   for (int i = 0; i < arrayamount; i++)
//   {
//      // Temperature.add(0);
//      // Humidity.add(0);     
//   }
}

void setjsonavgdata(String mm, double *t, double *h)   // 設定 JSON 資料
{
    StaticJsonDocument<1000> json_doc;  // 宣告一個靜態的 JSON 文件大小為 2000 bytes
    json_doc["Device"] = mm;            // 設定 "Device" 欄位為裝置 MAC 地址
    tempsum = 0;
    humidsum = 0;
    
    for (int i = 0; i < arrayamount; i++)   // 將溫度和濕度資料加入 JSON 陣列
    {
        tempsum += *(t + i) ;
        humidsum += *(h + i) ;
    }
    Serial.print("Check Temp Sum:") ;
    Serial.print(tempsum) ;
       Serial.print("\n") ;
    Serial.print("Check Humid Sum:") ;
    Serial.print(humidsum) ;
       Serial.print("\n") ;
      // Double2Str(ReadTemperature(),2).toDouble(); // 讀取溫度並轉換
//    json_doc["Temperature"] =  tempsum/arrayamount ;
//    json_doc["Humidity"] =  humidsum/arrayamount ;
    json_doc["Temperature"] =  Double2Str(tempsum/arrayamount,2).toDouble(); // 讀取溫度並轉換 ;
    json_doc["Humidity"] =  Double2Str(humidsum/arrayamount,2).toDouble(); // 讀取濕度並轉換 ;
    serializeJson(json_doc, json_data);  // 將 JSON 文件序列化並儲存到 json_data 字串
}

void setjsondata(String mm, double *t, double *h)   // 設定 JSON 資料
{
    StaticJsonDocument<2000> json_doc;  // 宣告一個靜態的 JSON 文件大小為 2000 bytes
    json_doc["Device"] = mm;            // 設定 "Device" 欄位為裝置 MAC 地址

    JsonArray Temperature = json_doc.createNestedArray("Temperature"); // 建立溫度的 JSON 陣列
    JsonArray Humidity = json_doc.createNestedArray("Humidity");       // 建立濕度的 JSON 陣列
    json_doc["Device"] = mm;

    for (int i = 0; i < arrayamount; i++)   // 將溫度和濕度資料加入 JSON 陣列
    {
        Temperature.add(*(t + i)); // 加入溫度值到 "Temperature" 陣列中
        Humidity.add(*(h + i));    // 加入濕度值到 "Humidity" 陣列中
    }
   
    serializeJson(json_doc, json_data);  // 將 JSON 文件序列化並儲存到 json_data 字串
}

void appendjsondata(Sensor dd, double *t, double *h, int pp)   // 將新數據加入溫溼度陣列
{
    *(t + pp) = dd.Temperature;   // 設定第 pp 筆的溫度值為感測到的溫度
    *(h + pp) = dd.Humidity;      // 設定第 pp 筆的濕度值為感測到的濕度
}
