#include <PubSubClient.h>   //MQTT 函式庫
#include <ArduinoJson.h>    //Json 使用元件

WiFiClient WifiClient;    //　ｗｅｂ　ｓｏｃｋｅｔ　元件

PubSubClient mqttclient(WifiClient) ;   //　MQTT Broker　元件 ，用PubSubClient類別產生一個 MQTT物件
//名稱為mqttclient，使用 WifiClient 的網路連線端


const char* PubTop = "/ncnu/DataCollector/%s" ;
const char* SubTop = "/ncnu/DataCollector/%s" ;
String TopicT;
char SubTopicbuffer[200];
char PubTopicbuffer[200];

#define MQTTServer "broker.emqx.io" 
#define MQTTPort 1883
char* MQTTUser = "";  // 不須帳密
char* MQTTPassword = "";    // 不須帳密
//char buffer[8000];

char clintid[20];
void mycallback(char* topic, byte* payload, unsigned int length)  ;
 void connectMQTT() ;
 
void fillCID(String mm)
{
    // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
      mm.toCharArray(&clintid[2],mm.length()+1) ;
    clintid[2+mm.length()+1] = '\n' ;


}


void fillTopic(String mm)
{
  sprintf(PubTopicbuffer,PubTop,mm.c_str()) ;
  sprintf(SubTopicbuffer,SubTop,mm.c_str()) ;
      Serial.print("Publish Topic Name:(") ;
    Serial.print(PubTopicbuffer) ;
    Serial.print(") \n") ;
      Serial.print("Subscribe Topic Name:(") ;
    Serial.print(SubTopicbuffer) ;
    Serial.print(") \n")  ;    
}




void initMQTT()   //MQTT Broker 初始化連線
{
 mqttclient.setServer(MQTTServer, MQTTPort);
  //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
  //mq.tongxinmao.com:18832
  mqttclient.setCallback(mycallback);
  // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
      
//--------------------------    
     fillCID(MacData); // generate a random clientid based MAC
  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  
  mqttclient.setServer(MQTTServer, MQTTPort);   // 設定MQTT Server  URL and Port
  mqttclient.setCallback(mycallback); //設定MQTT 回叫系統使用的函式:mycallback
  connectMQTT();    //連到MQTT Server
  Serial.println("Connecting to MQTT Server") ;
  
}

 void connectMQTT()
 {

  Serial.print("MQTT ClientID is :(") ;
  Serial.print(clintid) ;
  Serial.print(")\n") ;
  //印出MQTT Client基本訊息
  while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword))  //沒有連線
  {
      Serial.print("-");    //印出"-"
      delay(1000);
    }
    Serial.print("\n");

  mqttclient.subscribe(SubTopicbuffer); //訂閱我們的主旨
  Serial.println("Connect MQTT Server is OK") ;

  // client.unsubscribe("/hello");
}

void mycallback(char* topic, byte* payload, unsigned int length) 
{
  //mycallback(char* topic, byte* payload, unsigned int length)  參數格式固定，勿更改

    String payloadString;  // 將接收的 payload 轉成字串
    // 顯示訂閱內容
    Serial.print("Incoming:(") ;
    for (int i = 0; i < length; i++) 
    {
      payloadString = payloadString + (char)payload[i];
      //buffer[i]= (char)payload[i] ;
      Serial.print(payload[i],HEX) ;
    } 
    Serial.print(")\n") ;  
  
  payloadString = payloadString + '\0';  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] \n");
  
  //---------------------
  Serial.print("Content [");
  Serial.print(payloadString);
  Serial.print("] \n");
}
