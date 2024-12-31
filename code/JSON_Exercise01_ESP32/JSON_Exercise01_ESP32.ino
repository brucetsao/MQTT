#include <ArduinoJson.h>  //Json 使用元件

void setup() 
{
  // Initialize serial port
  Serial.begin(9600);   //監控視窗通訊速度為9600 bps
  while (!Serial) continue; //沒有開啟監控視窗，就停在這

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the capacity of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> doc;

  // StaticJsonDocument<N> allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument doc(200);

  // JSON input string.
  //
  // Using a char[], as shown here, enables the "zero-copy" mode. This mode uses
  // the minimal amount of memory because the JsonDocument stores pointers to
  // the input buffer.
  // If you use another type of input, ArduinoJson must copy the strings from
  // the input to the JsonDocument, so you need to increase the capacity of the
  // JsonDocument.
  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  //Json字元陣列
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  //使用deserializeJson方法，將字元陣列json[]的內容讀入，轉到doc json物件內，是否成功，回傳成功與否

  // Test if parsing succeeds.
  if (error)  //是否轉換成功之回傳變數
  {
    Serial.print(F("deserializeJson() failed: "));  //印出轉換失敗
    Serial.println(error.f_str());    //印出失敗原因碼
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = doc["sensor"]; 
  //宣告讀取json物件之文字變數：sensor
  //json物件之文字變數：sensor讀取 doc之json物件之sensor變數
  long time = doc["time"];
  //宣告讀取json物件之數字變數：time
  //json物件之數字變數：time讀取 doc之json物件之time變數
  double latitude = doc["data"][0];
    //宣告讀取json物件之陣列變數：data
  //json物件之陣列變數：data之第一個元素讀取 doc之json物件之data變數陣列第一個元素
  //將第一個元素存到 浮點變數 latitude 內
  double longitude = doc["data"][1];
    //宣告讀取json物件之陣列變數：data
  //json物件之陣列變數：data之第二個元素讀取 doc之json物件之data變數陣列第二個元素
  //將第二個元素存到 浮點變數 longitude 內
  // Print values.
  Serial.println(sensor); //印出sensor變數內容
  Serial.println(time); //印出 time 變數內容
  Serial.println(latitude, 6);  //印出 latitude 變數內容
  Serial.println(longitude, 6); //印出 longitude 變數內容
}

void loop() {
  // not used in this example
}
