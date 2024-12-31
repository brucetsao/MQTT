#include <String.h>   // String 使用必備函式庫
#include "initPins.h"   // 初始化腳位模組
#include "OledLib.h"  // Oled LCD 128x32 顯示模組
#include "HTU21DLib.h"  // HTU21D 溫濕度感測模組 使用必備函式庫
#include "MQTT.h" // MQTT Broker 自訂模組
#include "JSONLIB.h"   // Arduino JSON 使用必備函式庫

//--------------------
void connectMQTT();   // MQTT Broker 伺服器連線函式
void mycallback(char* topic, byte* payload, unsigned int length);  // MQTT Broker 伺服器回傳函式
void initAll();    // 系統初始化
//----------------

// setup 函式在按下重置或供電時執行一次
void setup() 
{
    initAll();   // 系統初始化

    initMQTT();  // MQTT Broker 初始化連線
    arraycount = 0;   // 設定 arraycount 初始值為 0
    initjson();   // 初始化 JSON 函式
    Serial.println("System Ready");   // 印出 "System Ready"
 //-----------------
}

// loop 函式持續重複執行
void loop() 
{
    if (arraycount >= arrayamount)   // 當 arraycount 達到 arrayamount 時
    {
        //setjsondata(MacData, &Temp[0], &Humid[0]);   // 設定 JSON 資料為 MAC 位址及溫濕度資料
        setjsonavgdata(MacData, &Temp[0], &Humid[0]);   // 設定 JSON 資料為 MAC 位址及溫濕度資料
        Serial.print("Now Json Data:"); 
        Serial.println(json_data);   // 印出 JSON 資料
        
        // 檢查 MQTT 客戶端是否已連線
        if (!mqttclient.connected()) 
        {
            connectMQTT();   // 若未連線則重新連接
        }
        
        // 傳送 buffer 變數到 MQTT Broker，指定傳送到 mytopic
        mqttclient.publish(PubTopicbuffer, json_data);
        Serial.print("Topic:");  
        Serial.print(PubTopicbuffer);
        Serial.print("\nPayload(");
        Serial.print(json_data);
        Serial.print(")\n"); 
        
        arraycount = 0;  // 重置 arraycount
        return;  
    } 
    else
    {
        Serial.print("Dn(");
        Serial.print(arraycount);
        Serial.print(")\n");
        
        onesensor = ReadSensor(onesensor);  // 讀取溫濕度感測數據
        _clearOled();   // 清除螢幕內容
        printTemperatureonOled(onesensor.Temperature);  // 將溫度顯示在 Oled 128x32 模組上
        printHumidityonOled(onesensor.Humidity);   // 將濕度顯示在 Oled 128x32 模組上
        _updateBuffer();    // 更新螢幕圖形記憶體的內容到畫面上
        appendjsondata(onesensor, &Temp[0], &Humid[0], arraycount);  // 添加溫濕度數據到 JSON
        arraycount++;   // 增加 arraycount 值
    }
    
    if (!mqttclient.connected()) 
    {
        connectMQTT();   // 若未連線則重新連接
    }
    
    mqttclient.loop(); // 處理 MQTT 多工程序
    delay(loopdelay);   // 延遲時間
}

void initAll()     // 系統初始化函式
{
    Serial.begin(9600);   // 設定 Serial 通訊波特率為 9600
    Serial.println("System Start");   // 印出 "System Start"
    
    MacData = GetMacAddress(); // 取得網路卡 MAC 位址
    initWiFi();  // 網路連線，連接到熱點
    ShowInternet();   // 顯示網路卡位址及連線資訊

    initHTU21D();   // 啟動 HTU21D 溫濕度感測器
    fillCID(MacData);   // 使用 MAC 位址填充 CID 資料
    fillTopic(MacData);   // 使用 MAC 位址填充 Topic 資料
    initOLED();   // 啟動 OLED 128x32 顯示模組
}
