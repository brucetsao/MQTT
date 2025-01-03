#include <String.h>   //String 使用必備函示庫
#include "initPins.h" //宣告開發版所有GPIO與WIFI與共用函式區
#include "MQTT.h"   //MQTT Broker與Json處理等函式區

//--------------------
 void connectMQTT() ; // MQTT 連線函數
 void mycallback(char* topic, byte* payload, unsigned int length)  ;   // 訂閱訊息的回傳呼叫函數
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

   if (!mqttclient.connected())   //如果MQTT 斷線(沒有連線)
  {
   
     Serial.println("connectMQTT  again");  //印出 "connectMQTT  again"

     connectMQTT();   //重新與MQTT Server連線


  }
 
  mqttclient.loop();    //處理MQTT 通訊處理程序
  //給作業系統處理多工程序的機會
  delay(300) ;
}

void initAll()     //系統初始化
{
    Serial.begin(9600) ;  //開啟監控通訊埠，並設定9600 bps通訊速率
    Serial.println("System Start");//印出System Start 訊息
   MacData = GetMacAddress() ; //取得網路卡編號
   initGPIO() ; //GPIO腳位初始化
}





//--------------------
