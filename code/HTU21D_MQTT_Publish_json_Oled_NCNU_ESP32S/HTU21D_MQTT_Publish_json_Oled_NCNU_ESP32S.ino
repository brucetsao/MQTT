#include <String.h>   //String 使用必備函示庫
#include "initPins.h"
#include "OledLib.h"  // Oled LCD 12832
#include "HTU21DLib.h"  //HTU21D 溫濕度感測模組 使用必備函示庫
#include "MQTT.h" // MQTT Broker自訂模組
#include "JSONLIB.h"   //arduino json 使用必備函示庫

//--------------------
 void connectMQTT() ;   //MQTT Broker伺服器連線函數
 void mycallback(char* topic, byte* payload, unsigned int length)  ;     //MQTT Broker伺服器回傳函數
 void initAll() ;    //系統初始化
//----------------

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
    initAll() ;   //系統初始化

    initMQTT()  ;//MQTT Broker 初始化連線
     arraycount = 0;
     initjson();
  Serial.println("System  Ready");
 //-----------------
}

// the loop function runs over and over again forever
void loop() 
{
     if (arraycount >= arrayamount)
      {
        setjsondata(MacData,&Temp[0],&Humid[0]) ;
    //    serializeJson(json_row,Serial) ;   
   //     Serial.println("") ;   
      Serial.print("Now Json Data:") ; 
        Serial.println(json_data) ;

      //產生 溫溼度資料，並轉到json_data變數
             if (!mqttclient.connected()) 
            {
               connectMQTT();
            }
            mqttclient.publish(PubTopicbuffer, json_data);    //傳送buffer變數到MQTT Broker，指定mytopic傳送
            Serial.print("Topic:")  ;
            Serial.print(PubTopicbuffer)  ;
            Serial.print("\nPayload(")  ;
            Serial.print(json_data) ;
            Serial.print(")\n")     ;       
          arraycount=0;
         // Temperature.clear();
         // Humidity.clear();  
         return ;    
      } 
      else
      {
           Serial.print("Dn(") ;
          Serial.print(arraycount) ;
          Serial.print(")\n") ;
          onesensor = ReadSensor(onesensor)  ; //讀取溫溼度
          _clearOled(); //清除螢幕
          printTemperatureonOled(onesensor.Temperature) ;  //列印溫度到Oled 12832顯示模組
          printHumidityonOled(onesensor.Humidity) ;   //列印濕度到Oled 12832顯示模組
          _updateBuffer();    //更新圖型記憶體內容到畫面
         // AppendSensorData(onesensor,sensordata[arraycount]) ;
          appendjsondata(onesensor,&Temp[0],&Humid[0],arraycount) ;
          arraycount++ ;

      }
    if (!mqttclient.connected()) 
  {
     connectMQTT();
  }
    
     mqttclient.loop(); //給作業系統處理多工程序的機會
  delay(loopdelay) ;
}

void initAll()     //系統初始化
{
    Serial.begin(9600) ;
    Serial.println("System Start");
   MacData = GetMacAddress() ; //取得網路卡編號

      initWiFi() ;  //網路連線，連上熱點
   ShowInternet() ;    //秀出網路卡編號

     initHTU21D();   //啟動HTU21D 溫溼度感測器
     fillCID(MacData) ;
     fillTopic(MacData) ;
      initOLED();   //啟動OLED 12832顯示模組
}
