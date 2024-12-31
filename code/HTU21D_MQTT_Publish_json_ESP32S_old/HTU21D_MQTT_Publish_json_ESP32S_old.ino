#include <String.h>   //String 使用必備函示庫
#include "initPins.h"
#include "HTU21DLib.h"
#include "MQTT.h" // MQTT Broker自訂模組
#include "JSONLIB.h"   

//--------------------
 void connectMQTT() ;
 void mycallback(char* topic, byte* payload, unsigned int length)  ;
//----------------

// the setup function runs once when you press reset or power the board
void setup() 
{
  
  // initialize digital pin LED_BUILTIN as an output.
    initAll() ;   //系統初始化

    initMQTT()  ;//MQTT Broker 初始化連線
     arraycount = 0;
 
  Serial.println("System  Ready");
 //-----------------

}

// the loop function runs over and over again forever
void loop() 
{
     if (arraycount >= arrayamount)
      {
        setjsondata(MacData) ;
        serializeJson(json_doc,json_data) ;   
        Serial.println("") ;    
        Serial.println(json_data) ;

      //產生 溫溼度資料，並轉到json_data變數

            mqttclient.publish(PubTopicbuffer, json_data);    //傳送buffer變數到MQTT Broker，指定mytopic傳送
            Serial.print("Sending  --->")  ;
            Serial.print(json_data) ;
            Serial.print("<--\n")     ;       
         if (!mqttclient.connected())   //如果MQTT 斷線(沒有連線)
        {
         
           Serial.println("connectMQTT  again");  //印出 "connectMQTT  again"
      
           connectMQTT();   //重新與MQTT Server連線
      
      
        }
        
        arraycount=0;
        json_row.clear();
      } 
      onesensor = ReadSensor(onesensor)  ; //讀取溫溼度
      AppendSensorData(onesensor,sensordata[arraycount]) ;
      appendjsondata(onesensor) ;
      arraycount++ ;
      Serial.print("Dn(") ;
      Serial.print(arraycount) ;
      Serial.print(")\n") ;
      
 
  mqttclient.loop();    //處理MQTT 通訊處理程序
  //給作業系統處理多工程序的機會
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
}





//--------------------
