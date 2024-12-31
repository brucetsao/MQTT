/*
 這段程式碼的註解解釋了每個函式的用途及主要程式邏輯，
 幫助了解程式如何操作 HTU21D 溫溼度感測器並將資料顯示在 OLED 上。
 */
#include <Wire.h>   //I2C 基礎通訊函式庫
#include "Adafruit_HTU21DF.h"   // HTU21D 溫溼度感測器函式庫
  
// 將 Vin 連接到 3-5V 直流電源
// 將 GND 連接到接地
// 將 SCL 連接到 I2C 的時鐘腳位 (UNO 的 A5)
// 將 SDA 連接到 I2C 的資料腳位 (UNO 的 A4)

//double Temp_Value = 0 ;
//double Humid_Value = 0 ;

Adafruit_HTU21DF htu = Adafruit_HTU21DF();  // 產生 HTU21D 溫溼度感測器的物件

char tmpvalue[100]; // 儲存溫溼度資料的字串

struct Sensor 
{   // 宣告結構體
  double Temperature = 0; // 溫度
  double Humidity = 0;    // 濕度
  
}; // 結束結構體宣告

Sensor onesensor;           // 單一感測器資料
Sensor sensordata[50];      // 儲存多筆感測器資料

void initHTU21D()   // 啟動 HTU21D 溫溼度感測器
{
    if (!htu.begin())   // 如果 HTU21D 溫溼度感測器無法啟動
    {
      Serial.println("Couldn't find sensor!"); // 印出訊息提示找不到感測器
      while (1);  // 進入無限迴圈
    }
}

float ReadTemperature() // 讀取 HTU21D 溫溼度感測器的溫度
{
    return htu.readTemperature();
}

float ReadHumidity() // 讀取 HTU21D 溫溼度感測器的濕度
{
    return htu.readHumidity();
}

Sensor ReadSensor()   // 讀取溫溼度資料
{
    Sensor ss ;
    ss.Temperature = Double2Str(ReadTemperature(),2).toDouble(); // 讀取溫度並轉換
    ss.Humidity = Double2Str(ReadHumidity(),2).toDouble(); // 讀取濕度並轉換
    Serial.print("Temp: ");               // 印出 "Temp: "
    Serial.print(ss.Temperature);         // 印出溫度數值
    Serial.print(" C");                   // 印出 " C"  
    Serial.print("\t\t");                 // 印出 "\t\t"
    Serial.print("Humidity: ");           // 印出 "Humidity: "
    Serial.print(ss.Humidity);            // 印出濕度數值
    Serial.println(" \%");                // 印出 " %"

    return ss;
}

void AppendSensorData(Sensor s1, Sensor s2)  // 將一筆感測器資料附加到另一筆
{
    s2.Temperature = s1.Temperature; // 傳遞溫度資料
    s2.Humidity = s1.Humidity;       // 傳遞濕度資料  
}

void printTemperatureonOled(float t) // 在 OLED 顯示溫度資料
{
      sprintf(tmpvalue, "%5.1f", t);  // 格式化溫度資料
      Serial.print("Data: ");
      Serial.print(tmpvalue);
      Serial.print("\n");
      
      _TPrint(1, 15, "Temp:");       // 顯示 "Temp:"
      _TPrint(50, 15, tmpvalue);     // 顯示溫度數值
      _TPrint(100, 15, ".C");        // 顯示 ".C"
}

void printHumidityonOled(float h) // 在 OLED 顯示濕度資料
{
      sprintf(tmpvalue, "%5.1f", h);  // 格式化濕度資料
      Serial.print("Data: ");
      Serial.print(tmpvalue);
      Serial.print("\n");
      
      _TPrint(1, 31, "Humid:");      // 顯示 "Humid:"
      _TPrint(60, 31, tmpvalue);     // 顯示濕度數值
      _TPrint(110, 31, "%");         // 顯示 "%"
}
