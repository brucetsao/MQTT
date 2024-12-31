#include <PubSubClient.h>   //MQTT 函式庫
#include <ArduinoJson.h>    //Json 使用元件

WiFiClient WifiClient;    // 網路 socket 元件

PubSubClient mqttclient(WifiClient) ;   // MQTT Broker 元件，用 PubSubClient 類別產生一個 MQTT 物件
//名稱為 mqttclient，使用 WifiClient 的網路連線端

#define mytopic "/nuk/sensor/"    // 訂閱的主題，同學請根據需求更改
#define mytopicA "/ncnu/TeamA"    // 訂閱的主題，同學請根據需求更改
#define mytopicB "/ncnu/TeamB"    // 訂閱的主題，同學請根據需求更改
#define mytopicC "/ncnu/TeamC"    // 訂閱的主題，同學請根據需求更改

#define MQTTServer "broker.emqx.io" // MQTT Broker 地址
#define MQTTPort 1883                // MQTT Broker 埠號
char* MQTTUser = "";  // 無需帳號密碼
char* MQTTPassword = "";    // 無需帳號密碼

// JSON 資料處理所需的變數
char json_data[2000];
StaticJsonDocument<2000> json_doc;
char* cmdon = "ON" ;  // 指令: 開啟
char* cmdoff = "OFF" ; // 指令: 關閉

char buffer[400]; // 暫存 JSON 資料

String SubTopic =String("/nuk/sensor/#") ; // 訂閱所有子主題
String FullTopic ;
char fullTopic[35] ;

char clintid[20]; // MQTT Client ID
void mycallback(char* topic, byte* payload, unsigned int length);  // 訂閱訊息的回呼函數
void connectMQTT(); // MQTT 連線函數

// 產生 Client ID
void fillCID(String mm)
{
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
  mm.toCharArray(&clintid[2], mm.length()+1);
  clintid[2+mm.length()+1] = '\n';
}

// 設定完整主題
void fillTopic(String mm)
{
  mm.toCharArray(&fullTopic[0], mm.length()+1);
  fullTopic[mm.length()+1] = '\n';
}

// 產生 JSON 資料格式
void Genjsondata(String myid, String myname, float temp, float humid)
{
  StaticJsonDocument<400> doc;    // 產生一個 JSON 物件，大小為 400 字元

  // 設定 JSON 內容
  doc["id"] = myid;
  doc["name"] = myname;
  doc["temperature"] = String(temp);
  doc["humidity"] = String(humid);

  serializeJson(doc, buffer); // 將 JSON 資料序列化並存入 buffer
}

// MQTT 初始化設定
void initMQTT()
{
  mqttclient.setServer(MQTTServer, MQTTPort); // 設定 MQTT 伺服器與埠號
  mqttclient.setCallback(mycallback);         // 設定回呼函數

  fillCID(MacData); // 產生基於 MAC 的隨機 Client ID
  Serial.print("MQTT ClientID is :(");
  Serial.print(clintid);
  Serial.print(")\n");

  connectMQTT(); // 連線到 MQTT 伺服器
}

// 連線至 MQTT Broker
void connectMQTT()
{
  Serial.print("MQTT ClientID is :(");
  Serial.print(clintid);
  Serial.print(")\n");

  while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword)) // 嘗試連線
  {
      Serial.print("-");
      delay(1000); // 每秒重試一次
  }
  Serial.print("\n");

  Serial.print("String Topic:[");
  Serial.print(mytopic);
  Serial.print("]\n");

  Serial.print("char Topic:[");
  Serial.print(fullTopic);
  Serial.print("]\n");   

  mqttclient.subscribe(mytopic); // 訂閱指定的主題
  Serial.println("\n MQTT connected!");
}

// 接收訂閱訊息的回呼函數
void mycallback(char* topic, byte* payload, unsigned int length)
{
  String payloadString;  // 將接收的 payload 轉換為字串

  Serial.print("Incoming:(");
  for (int i = 0; i < length; i++) 
  {
    payloadString = payloadString + (char)payload[i];
    json_data[i] = (char)payload[i];
    Serial.print(payload[i], HEX);
  } 
  Serial.print(")\n");  

  json_data[length]  = '\0'; 
  payloadString = payloadString + '\0';  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] \n");

  Serial.print("Content [");
  Serial.print(payloadString);
  Serial.print("] \n");

  Serial.print("Content2 [");
  Serial.print(json_data);
  Serial.print("] \n");

  DeserializationError error = deserializeJson(json_doc, json_data); // 解序列化 JSON
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  const char* cmd = json_doc["Command"];
  Serial.print("The Command is :");
  Serial.print(cmd);
  Serial.print("\n");

  if (json_doc["Command"] == "ON")
  {
    LedON(); // 開啟 LED
  }
  if (json_doc["Command"] == "OFF")
  {
    LedOFF(); // 關閉 LED
  }
}
