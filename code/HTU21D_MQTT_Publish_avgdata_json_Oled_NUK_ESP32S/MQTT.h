/*
這段程式碼設定了與 MQTT 伺服器的
連線、主題的生成以及如何處理接收到的
訊息。註解詳述了每個步驟及函數的用途。
 */
#include <PubSubClient.h>   // MQTT 函式庫
#include <ArduinoJson.h>    // JSON 使用元件

WiFiClient WifiClient;    // WiFi 客戶端元件，用於網路連線
PubSubClient mqttclient(WifiClient);   // 使用 PubSubClient 類別創建一個 MQTT 物件，名稱為 mqttclient，並使用 WifiClient 的網路連線端

const char* PubTop = "/nuk/dht/%s";   // 發佈資料的主題 (Topic)
const char* SubTop = "/nuk/#";          // 訂閱資料的主題 (Topic)
String TopicT;        // 暫存的主題變數
char SubTopicbuffer[200];   // 用於訂閱主題的字元陣列變數，MQTT Broker 只能接受 char[] 陣列
char PubTopicbuffer[200];   // 用於發佈主題的字元陣列變數

#define MQTTServer "broker.emqx.io"  // MQTT Broker 伺服器的網址
#define MQTTPort 1883   // MQTT Broker 伺服器的通訊埠
char* MQTTUser = "";  // MQTT Broker 伺服器的使用者名稱（不須帳密）
char* MQTTPassword = "";    // MQTT Broker 伺服器的密碼（不須帳密）

char clintid[20];   // MQTT Broker 伺服器的連線 ID 陣列變數
void mycallback(char* topic, byte* payload, unsigned int length);   // MQTT Broker 回呼函式
void connectMQTT();     // 連線至 MQTT Broker 函式

void fillCID(String mm)   // 產生 MQTT Broker 伺服器的連線 ID
{
    // 以 MAC 地址生成一個隨機的客戶端 ID
    clintid[0] = 't';
    clintid[1] = 'w';
    mm.toCharArray(&clintid[2], mm.length() + 1);  // 將 MAC 地址轉換為字元陣列並存入 clintid
    clintid[2 + mm.length() + 1] = '\n';
}

void fillTopic(String mm)   // 產生 MQTT Broker 伺服器的發佈和訂閱主題
{
    sprintf(PubTopicbuffer, PubTop, mm.c_str());  // 使用 MAC 地址填充發佈主題
    sprintf(SubTopicbuffer, SubTop, mm.c_str());  // 使用 MAC 地址填充訂閱主題

    Serial.print("Publish Topic Name:(");
    Serial.print(PubTopicbuffer);
    Serial.print(") \n");
    Serial.print("Subscribe Topic Name:(");
    Serial.print(SubTopicbuffer);
    Serial.print(") \n");    
}

void initMQTT()   // 初始化 MQTT 連線
{
    mqttclient.setServer(MQTTServer, MQTTPort);    // 設定 MQTT Broker 主機與通訊埠
    mqttclient.setCallback(mycallback);   // 設定 MQTT Broker 回呼函式
    fillCID(MacData); // 生成基於 MAC 地址的客戶端 ID
    Serial.print("MQTT ClientID is :(");
    Serial.print(clintid);
    Serial.print(")\n");

    mqttclient.setServer(MQTTServer, MQTTPort);   // 再次設定 MQTT Broker 主機與通訊埠
    mqttclient.setCallback(mycallback); // 設定 MQTT 回呼函式
    connectMQTT();    // 連線至 MQTT Broker
    Serial.println("Connecting to MQTT Server");
}

void connectMQTT()  // 與 MQTT Broker 連線
{
    Serial.print("MQTT ClientID is :(");
    Serial.print(clintid);
    Serial.print(")\n");
    // 印出 MQTT 客戶端 ID 資訊
    while (!mqttclient.connect(clintid, MQTTUser, MQTTPassword))  // 若未連線則持續重試
    {
        Serial.print("-");    // 印出 "-" 表示連線嘗試
        delay(1000);
    }
    Serial.print("\n");

    mqttclient.subscribe(SubTopicbuffer); // 訂閱我們設定的主題
    Serial.println("Connect MQTT Server is OK");
}

void mycallback(char* topic, byte* payload, unsigned int length) 
{
    // MQTT 回呼函式，參數格式固定，勿更改

    String payloadString;  // 將接收到的 payload 轉換為字串
    Serial.print("Incoming:(");
    for (int i = 0; i < length; i++) 
    {
        payloadString += (char)payload[i];  // 將 payload 轉為字串
        Serial.print(payload[i], HEX);  // 以十六進制顯示每個字元
    } 
    Serial.print(")\n");  
  
    payloadString += '\0';  
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] \n");

    Serial.print("Content [");
    Serial.print(payloadString);
    Serial.print("] \n");
}
