
#include <Wire.h>   //I2C 基礎通訊函式庫
#include "Adafruit_HTU21DF.h"   // HTU21D 溫溼度感測器函式庫
  
// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (A5 on UNO)
// Connect SDA to I2C data pin (A4 on UNO)
//double Temp_Value = 0 ;
//double Humid_Value = 0 ;
Adafruit_HTU21DF htu = Adafruit_HTU21DF();  //產生HTU21D 溫溼度感測器運作物件
 //產生HTU21D 溫溼度感測器運作物件

struct Sensor 
{   // Structure declaration
  double Temperature = 0 ;//溫度
  double Humidity = 0 ;//濕度
  
}; // End the structure with a semicolon

Sensor onesensor ;
Sensor sensordata[20] ;

void initHTU21D()   //啟動HTU21D 溫溼度感測器
{
    if (!htu.begin())   //如果 HTU21D 溫溼度感測器沒有啟動成功
    {
      Serial.println("Couldn't find sensor!");
      //找不到 HTU21D 溫溼度感測器
      while (1);  //永遠死在這
    }
}

float ReadTemperature() //讀取HTU21D 溫溼度感測器之溫度
{
    return htu.readTemperature();
}
float ReadHumidity() //讀取HTU21D 溫溼度感測器之溼度
{
    return htu.readHumidity();
}


Sensor ReadSensor(Sensor ss)   //讀取溫溼度
{
    ss.Temperature = Double2Str(ReadTemperature(),2).toDouble(); //讀取HTU21D 溫溼度感測器之溫度
    ss.Humidity = Double2Str(ReadHumidity(),2).toDouble() ; //讀取HTU21D 溫溼度感測器之溼度
    Serial.print("Temp: ");               //印出 "Temp: "
    Serial.print(ss.Temperature);                   //印出 temp變數內容
    Serial.print(" C");                   //印出 " C"  
    Serial.print("\t\t");                 //印出 "\t\t"
    Serial.print("Humidity: ");           //印出 "Humidity: "
    Serial.print(ss.Humidity);                //印出 rel_hum變數內容
    Serial.println(" \%");                //印出 " \%"

  return ss;
}

void AppendSensorData(Sensor s1, Sensor s2)
{
    s2.Temperature = s1.Temperature; //讀取HTU21D 溫溼度感測器之溫度
    s2.Humidity = s1.Humidity; //讀取HTU21D 溫溼度感測器之溼度   
}

void printTemperatureonOled(float t) //讀取HTU21D 溫溼度感測器之溫度
{
      sprintf(tmpvalue,"%5.1f",t) ;
      Serial.print("Data:");
      Serial.print(tmpvalue);
      Serial.print("\n");
      
      _TPrint(1,15,"Temp:");
      _TPrint(50,15,tmpvalue);
      _TPrint(100,15,".C");
}

void printHumidityonOled(float h) //讀取HTU21D 溫溼度感測器之溫度
{
  
      sprintf(tmpvalue,"%5.1f",h) ;
      Serial.print("Data:");
      Serial.print(tmpvalue);
      Serial.print("\n");
      
      _TPrint(1,31,"Humid:");
      _TPrint(60,31,tmpvalue);
      _TPrint(110,31,"%");
}
