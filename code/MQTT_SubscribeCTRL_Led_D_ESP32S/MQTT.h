//外部函式庫宣告區
#include <PubSubClient.h>   //MQTT 函式庫
#include <ArduinoJson.h>    //Json 使用元件

//產生網路 socket 元件
WiFiClient WifiClient;    // 產生網路 socket 元件
// 產生MQTT Broker 元件
PubSubClient mqttclient(WifiClient) ;   // 產生MQTT Broker 元件，用 PubSubClient 類別產生一個 MQTT 物件
//名稱為 mqttclient，使用 WifiClient 的網路連線端

//MQTT Broker登錄必要資訊區
#define mytopic "/nuk/sensor/"    // 訂閱的主題，同學請根據需求更改
#define mytopicA "/ncnu/TeamA"    // 訂閱的主題，同學請根據需求更改
#define mytopicB "/ncnu/TeamB"    // 訂閱的主題，同學請根據需求更改
#define mytopicC "/ncnu/TeamC"    // 訂閱的主題，同學請根據需求更改

//MQTT Broker登錄必要資訊區
#define MQTTServer "broker.emqx.io" // MQTT Broker 地址
#define MQTTPort 1883                // MQTT Broker 埠號
char* MQTTUser = "";  // 無需帳號密碼
char* MQTTPassword = "";    // 無需帳號密碼

// JSON 資料處理所需的變數
//JSON 資料處理所需的變數區
char json_data[2000];
StaticJsonDocument<2000> json_doc;
char* cmdon = "ON" ;  // 指令: 開啟
char* cmdoff = "OFF" ; // 指令: 關閉
char buffer[400]; // 暫存 JSON 資料

//自訂函式宣告區
String SubTopic =String("/nuk/sensor/#") ; // 訂閱所有子主題
String FullTopic ;  //產生TOPIC所用的字串變數
char fullTopic[35] ;  //產生TOPIC所用的char 陣列變數
char clintid[20]; // MQTT Client ID

//自訂函式宣告區
void mycallback(char* topic, byte* payload, unsigned int length);  // 訂閱訊息的回傳呼叫函數
void connectMQTT(); // MQTT 連線函數

// 產生 Client ID
void fillCID(String mm)
{
  clintid[0]= 't' ;  //MQTT Broker 的ClientID第一個字母
  clintid[1]= 'w' ; //MQTT Broker 的ClientID第二個字母 
  mm.toCharArray(&clintid[2], mm.length()+1); //MQTT Broker 的ClientID第三個字母起用MAC Address代替
  clintid[2+mm.length()+1] = '\n';  //MQTT Broker 的ClientID所有字母完成後，加上\n表結束
}

// 設定完整TOPIC主題
void fillTopic(String mm) //因為MQTT Broker 的TOPIC需要用char 陣列，所以將字串轉char 陣列
{
  //因為MQTT Broker 的TOPIC需要用char 陣列，所以將字串轉char 陣列
  mm.toCharArray(&fullTopic[0], mm.length()+1); //因為MQTT Broker 的TOPIC需要用char 陣列，所以將字串轉char 陣列
  fullTopic[mm.length()+1] = '\n';  //MQTT Broker 的TOPIC完成後，加上\n表結束
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
void initMQTT() //MQTT Broker 初始化連線
{
  mqttclient.setServer(MQTTServer, MQTTPort); // 設定 MQTT 伺服器與埠號
  mqttclient.setCallback(mycallback);         // 設定回呼函數

  fillCID(MacData); // 產生基於 MAC 的隨機 Client ID
  Serial.print("MQTT ClientID is :(");
  Serial.print(clintid);  //印出ClientID
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
      Serial.print("-");  //印出"-"
      delay(1000); // 每秒重試一次
  }
  Serial.print("\n"); //印出換行鍵

  Serial.print("String Topic:["); //印出String Topic:[
  Serial.print(mytopic);  //印出TOPIC內容  
  Serial.print("]\n");  //印出換行鍵

  Serial.print("char Topic:[");//印出char Topic:[
  Serial.print(fullTopic);//印出TOPIC內容
  Serial.print("]\n");    //印出換行鍵

  mqttclient.subscribe(mytopic); // 訂閱指定的主題
  Serial.println("\n MQTT connected!"); //印出MQTT connected!
}

// 接收訂閱訊息的回呼函數
void mycallback(char* topic, byte* payload, unsigned int length)
{
  String payloadString;  // 將接收的 payload 轉換為字串

  Serial.print("Incoming:("); //印出換Incoming:(
  for (int i = 0; i < length; i++)  //用迴圈處理payload的byte streams的內容
  {
    payloadString = payloadString + (char)payload[i]; //將每一個byte 的字加入payloadString字串
    json_data[i] = (char)payload[i];  //將每一個byte 的字加入json_data[i]字元陣列
    Serial.print(payload[i], HEX);  //印出每一個payloadString的byte得16進位值
  } 
  Serial.print(")\n");  //印出換行鍵

  json_data[length]  = '\0';  //將\0字元加入json_data[i]字元陣列
  payloadString = payloadString + '\0';  //將\0字元加入jpayloadString字串
  Serial.print("Message arrived [");   //印出Message arrived [
  Serial.print(topic);   //印出topic變數內容
  Serial.print("] \n");  //印出換行鍵

  Serial.print("Content [");  //印出Content [
  Serial.print(payloadString);  //印出payloadString變數內容
  Serial.print("] \n"); //印出換行鍵

  Serial.print("Content2 ["); //印出Content [
  Serial.print(json_data);  //印出json_data[i]字元陣列內容
  Serial.print("] \n"); //印出換行鍵

  DeserializationError error = deserializeJson(json_doc, json_data); // 解序列化 JSON
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));  //印出解序列化 JSON錯誤
    Serial.println(error.c_str());  //印出錯誤碼
    return; //離開處理程序
  }

  const char* cmd = json_doc["Command"];  //取得json之"Command"元素內容
  Serial.print("The Command is :"); //印出The Command is :
  Serial.print(cmd);  //印出json之"Command"元素內容
  Serial.print("\n"); //印出換行鍵

  if (json_doc["Command"] == "ON")  //如果json之"Command"元素內容為"ON"
  {
    LedON(); // 開啟 LED
    Serial.println("Led is ON") ;   //印出Led is ON
  }
  if (json_doc["Command"] == "OFF") //如果json之"Command"元素內容為"OFF"
  {
    LedOFF(); // 關閉 LED
    Serial.println("Led is OFF") ;    //印出Led is OFF
  }
}
