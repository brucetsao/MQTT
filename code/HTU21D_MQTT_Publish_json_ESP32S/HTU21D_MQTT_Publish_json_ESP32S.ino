/*
 
開發物聯網（IoT）系統涉及多個步驟，
包括需求分析、硬體選擇、軟體開發、
網路連接、安全考慮、數據處理和應用整合。

 */
#include <String.h>   // String 使用必備函式庫
#include "initPins.h"
#include "HTU21DLib.h"  // HTU21D 溫濕度感測模組使用必備函式庫
#include "MQTT.h" // MQTT Broker 自訂模組
#include "JSONLIB.h"   // arduino json 使用必備函式庫

//--------------------
void connectMQTT(); // 連接 MQTT 函式
void mycallback(char* topic, byte* payload, unsigned int length); // MQTT 回呼函式
void initAll();    // 系統初始化函式
//----------------

// setup 函式在按下重設或上電時運行一次
void setup() 
{
  // 初始化數位腳位 LED_BUILTIN 為輸出模式
  initAll();   // 系統初始化
  initMQTT();  // MQTT Broker 初始化連線
  arraycount = 0; // 初始化計數器
  initjson();  // 初始化 JSON 函式
  Serial.println("System Ready"); // 輸出系統準備訊息
 //-----------------
}

// loop 函式會一直重複運行
void loop() 
{
  if (arraycount >= arrayamount) // 如果計數器達到設定的數量
  {
    setjsondata(MacData, &Temp[0], &Humid[0]); // 設定 JSON 資料
    Serial.print("Now Json Data:"); 
    Serial.println(json_data); // 輸出目前的 JSON 資料

    // 產生溫濕度資料，並轉到 json_data 變數
    if (!mqttclient.connected()) 
    {
      connectMQTT(); // 如果 MQTT 尚未連接，則連接
    }
    mqttclient.publish(PubTopicbuffer, json_data); // 傳送 buffer 變數到 MQTT Broker，指定 mytopic 傳送
    Serial.print("Topic:");  
    Serial.print(PubTopicbuffer);  
    Serial.print("\nPayload(");  
    Serial.print(json_data); 
    Serial.print(")\n");       
    arraycount = 0; // 重置計數器
    return;    
  } 
  else
  {
    Serial.print("Dn(");
    Serial.print(arraycount);
    Serial.print(")\n");
    onesensor = ReadSensor(onesensor); // 讀取溫濕度
    appendjsondata(onesensor, &Temp[0], &Humid[0], arraycount); // 將感測器資料附加到 JSON 資料
    arraycount++; // 計數器加一
  }

  if (!mqttclient.connected()) 
  {
    connectMQTT(); // 如果 MQTT 尚未連接，則連接
  }
    
  mqttclient.loop(); // 給作業系統處理多工程序的機會
  delay(loopdelay); // 延遲一段時間
}

void initAll() // 系統初始化函式
{
  Serial.begin(9600); // 初始化序列埠，設定波特率為 9600
  Serial.println("System Start"); // 輸出系統啟動訊息
  MacData = GetMacAddress(); // 取得網路卡編號

  initWiFi(); // 網路連線，連上熱點
  ShowInternet(); // 顯示網路卡編號

  initHTU21D(); // 啟動 HTU21D 溫濕度感測器
  fillCID(MacData); // 填充客戶端 ID
  fillTopic(MacData); // 填充主題
}
