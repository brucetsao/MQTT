void callback(char* topic, byte* payload, unsigned int length)  ;

#include "initPins.h"     // 腳位與系統模組
#include "MQTTLIB.h"   // MQTT Broker自訂模組


void setup()
{

    initALL() ; //系統硬體/軟體初始化
    initWiFi() ;  //網路連線，連上熱點
    ShowInternet();  //秀出網路連線資訊
   
    initMQTT() ;    //起始MQTT Broker連線
    connectMQTT();    //連到MQTT Server 
    delay(1000) ;
}

void loop()
{
 if (!mqttclient.connected()) 
  {
     connectMQTT();
  }
    double Temp_Value = 25+(double)(random(-300, 300)/50);
    double Humid_Value= 70 +(double)(random(-300, 300)/10);
    String msg ="目前溫度:"+String(Temp_Value)+"°C, 目前濕度："+String(Humid_Value);
    Serial.print("Temp: "); Serial.print(Temp_Value); Serial.print(" C");
    Serial.print("\t\t");
    Serial.print("Humidity: "); Serial.print(Humid_Value); Serial.println(" \%");
    StatusPublish(MacData, Temp_Value,Humid_Value) ;
    // CheckWifiLed() ;
   mqttclient.loop();
       delay(loopdelay) ;
}

/* Function to print the sending result via Serial */

void initALL()  //系統硬體/軟體初始化
{
    Serial.begin(9600);
    Serial.println("System Start");

    MacData = GetMacAddress() ;   //取得網路卡編號
    fillCID(MacData) ;
    fillTopic(MacData) ;
}

 void connectMQTT()
 {

  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  //印出MQTT Client基本訊息
  while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword)) 
  {
//  while (!mqttclient.connect(clintid)) {
    Serial.print("-");
    delay(1000);
  }
    Serial.print("\n");

  mqttclient.subscribe(SubTopicbuffer); //訂閱我們的主旨
  Serial.println("Connect MQTT Server is OK") ;
}

void callback(char* topic, byte* payload, unsigned int length) 
{
   Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] \n");
  deserializeJson(doc, payload, length);
  JsonObject documentRoot = doc.as<JsonObject>();
 // Serial.print("Device:") ;
  const char* a1 = documentRoot.getMember("Device") ;
 // Serial.println(a1);
  double a2 = documentRoot.getMember("Temperature") ;
  //Serial.println(a2);
  double a3 = documentRoot.getMember("Humidity") ;
 // Serial.println(a3);
 Serial.print("Received from MAC:");
 Serial.println(a1) ;
 Serial.print("Received Temperature:");
 Serial.println(a2) ;
 Serial.print("Received Humidity:");
 Serial.println(a3) ;
 

}
