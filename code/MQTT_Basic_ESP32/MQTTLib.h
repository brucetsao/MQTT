#include <ArduinoJson.h>
#include <PubSubClient.h>

#define MQTTServer "broker.emqx.io" 
#define MQTTPort 1883
char* MQTTUser = "";  // 不須帳密
char* MQTTPassword = "";    // 不須帳密

WiFiClient mqclient ;  //　ｗｅｂ　ｓｏｃｋｅｔ　元件
PubSubClient mqttclient(mqclient)  ;   //　MQTT Broker　元件 ，用PubSubClient類別產生一個 MQTT物件
StaticJsonDocument<512> doc;
char JSONmessageBuffer[300];
String payloadStr ;

//----------------
#include <ArduinoJson.h>  //Json 使用元件
//MQTT Server Use
const char* PubTop = "/ncnu/DataCollector/%s" ;
const char* SubTop = "/ncnu/DataCollector/#" ;
String TopicT;
char SubTopicbuffer[200];
char PubTopicbuffer[200];

//Publish & Subscribe use
const char* PrePayload = "{\"Device\":\"%s\",\"Temperature\":%f,\"Humidity\":%f}" ;
String PayloadT;
char Payloadbuffer[250];
char clintid[20];



#define MQTT_RECONNECT_INTERVAL 100                   // millisecond
#define MQTT_LOOP_INTERVAL      50                    // millisecond


void fillCID(String mm)
{
    // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
      mm.toCharArray(&clintid[2],mm.length()+1) ;
    clintid[2+mm.length()+1] = '\n' ;
    Serial.print("Client ID:(") ;
    Serial.print(clintid) ;
    Serial.print(") \n") ;
    
}

void fillTopic(String mm)
{
  sprintf(PubTopicbuffer,PubTop,mm.c_str()) ;
      Serial.print("Publish Topic Name:(") ;
    Serial.print(PubTopicbuffer) ;
    Serial.print(") \n") ;
  sprintf(SubTopicbuffer,SubTop,mm.c_str()) ;
      Serial.print("Subscribe Topic Name:(") ;
    Serial.print(SubTopicbuffer) ;
    Serial.print(") \n")  ;
}

void fillPayload(String dev, double d1, double d2)
{
  sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; ;
      Serial.print("Payload Content:(") ;
    Serial.print(Payloadbuffer) ;
    Serial.print(") \n") ;
}


void initMQTT()
{
    mqttclient.setServer(MQTTServer, MQTTPort);
    Serial.println("Now Set MQTT Server") ;
  //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
  //mq.tongxinmao.com:18832
  mqttclient.setCallback(callback);
  // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
      
//--------------------------    

}

//--------------
void StatusPublish(String mm,double d1, double d2)    //Publish System
{
      fillPayload(mm, d1,d2) ;
      mqttclient.publish(PubTopicbuffer,Payloadbuffer);
}
