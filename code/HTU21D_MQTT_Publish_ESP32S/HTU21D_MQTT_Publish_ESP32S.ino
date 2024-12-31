#include <String.h>   //String 使用必備函示庫
#include "initPins.h"
#include "HTU21DLib.h"
#include "MQTT.h"

//--------------------
 void connectMQTT() ;
 void mycallback(char* topic, byte* payload, unsigned int length)  ;
//----------------

// the setup function runs once when you press reset or power the board
void setup() 
{
  
  // initialize digital pin LED_BUILTIN as an output.
    initAll() ;   //系統初始化
    initWiFi() ;  //網路連線，連上熱點
   ShowInternet() ;    //秀出網路卡編號
    initMQTT()  ;//MQTT Broker 初始化連線
 
 
  Serial.println("System  Ready");
 //-----------------

}

// the loop function runs over and over again forever
void loop() 
{
      ReadSensor()  ; //讀取溫溼度

      Genjsondata(MacData,"BruceTsao",ReadTemperature(),ReadHumidity()) ;  
      //產生 溫溼度資料，並轉到buffer變數
      Serial.println(buffer) ;    //印出buffer變數
      mqttclient.publish(mytopic, buffer);    //傳送buffer變數到MQTT Broker，指定mytopic傳送
      Serial.println(mytopic);    //Output to Serial var_mytopic
      Serial.println(buffer);    //Output to Serial var_buffer
      
   if (!mqttclient.connected())   //如果MQTT 斷線(沒有連線)
  {
   
     Serial.println("connectMQTT  again");  //印出 "connectMQTT  again"

     connectMQTT();   //重新與MQTT Server連線


  }
 
  mqttclient.loop();    //處理MQTT 通訊處理程序
  //給作業系統處理多工程序的機會
  delay(10000) ;
}

void initAll()     //系統初始化
{
    Serial.begin(9600) ;
    Serial.println("System Start");
   MacData = GetMacAddress() ; //取得網路卡編號
  initHTU21D();   //啟動HTU21D 溫溼度感測器
  
}





//--------------------
