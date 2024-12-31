#define _Debug 1    // 輸出偵錯訊息
#define _debug 1    // 輸出偵錯訊息
#define initDelay   6000    // 初始化延遲時間 (毫秒)
#define loopdelay 2000   // 主程式循環延遲時間 (毫秒)

#define terminator '\n'   // 指令結尾符號為換行符
int ccmd = -1 ;  // 用於接收命令的變數
String cmdstr ;  // 用於儲存指令字串的變數

#include <WiFi.h>   // 使用 WiFi 函式庫
#include <WiFiClient.h>   // 使用 WiFi 用戶端函式庫
#include <WiFiMulti.h>    // 多熱點網路函式庫

WiFiMulti wifiMulti;    // 產生多熱點連線物件

String IpAddress2String(const IPAddress& ipAddress);   // 將 IPAddress 轉為字串
IPAddress ip;    // 儲存 WiFi 取得的 IP 位址的變數
String IPData;   // 儲存 IP 位址的字串變數
String APname;   // 儲存 WiFi 熱點名稱的變數
String MacData;   // 儲存網路卡的 MAC 位址的變數 
long rssi;   // 儲存 WiFi 連線訊號強度的變數
int status = WL_IDLE_STATUS;  // 儲存 WiFi 連線狀態的變數

void initWiFi()   // 初始化並連接 WiFi 熱點
{
  // 加入熱點資料
  wifiMulti.addAP("lab309", "");  // 加入 WiFi 熱點 lab309
  wifiMulti.addAP("Lab203", "203203203");  // 加入 WiFi 熱點 lab203
  wifiMulti.addAP("NUKIOT", "iot12345");  // 加入 WiFi 熱點 NUKIOT
  wifiMulti.addAP("NCNUIOT", "12345678");  // 加入 WiFi 熱點 NCNUIOT
  wifiMulti.addAP("NCNUIOT2", "12345678");  // 加入 WiFi 熱點 NCNUIOT2
  wifiMulti.addAP("ABC", "12345678");  // 加入 WiFi 熱點 ABC

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");   // 通訊埠印出 "Connecting to "
  
  wifiMulti.run();  // 啟動多熱點連線
  while (WiFi.status() != WL_CONNECTED)   // 若還未連線成功
  {
    Serial.print(".");   // 通訊埠印出點 (.)
    delay(500);  // 延遲 500 ms
    wifiMulti.run();   // 多網路熱點設定連線
  }
  
  Serial.println("WiFi connected");   // 印出 "WiFi connected"
  Serial.print("AP Name: ");   // 印出 "AP Name: "
  APname = WiFi.SSID();   // 取得並儲存熱點名稱
  Serial.println(APname);   // 印出熱點名稱
  Serial.print("IP address: ");   // 印出 "IP address: "
  ip = WiFi.localIP();   // 取得並儲存 IP 位址
  IPData = IpAddress2String(ip);   // 將 IP 位址轉為字串
  Serial.println(IPData);   // 印出 IP 位址
}

void ShowInternet()   // 顯示網路連線資訊
{
  Serial.print("MAC:");   // 印出 "MAC:"
  Serial.print(MacData);   // 印出 MAC 位址
  Serial.print("\n");
  Serial.print("SSID:");   // 印出 "SSID:"
  Serial.print(APname);   // 印出熱點名稱
  Serial.print("\n");
  Serial.print("IP:");   // 印出 "IP:"
  Serial.print(IPData);   // 印出 IP 位址
  Serial.print("\n");    
}

//--------------------

long POW(long num, int expo)   // 自訂指數函數
{
  long tmp = 1;
  if (expo > 0)
  { 
    for(int i = 0; i < expo; i++)
      tmp = tmp * num;
    return tmp; 
  } 
  else
  {
    return tmp; 
  }
}

String SPACE(int sp)   // 返回指定數量的空格字串
{
  String tmp = "";
  for (int i = 0; i < sp; i++)
  {
    tmp.concat(' ');
  }
  return tmp;
}

String strzero(long num, int len, int base)   // 將數字轉為指定長度與進制的字串
{
  String retstring = "";
  int ln = 1;
  int i = 0; 
  char tmp[10];
  long tmpnum = num;
  int tmpchr = 0;
  char hexcode[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  
  while (ln <= len)
  {
    tmpchr = (int)(tmpnum % base);
    tmp[ln - 1] = hexcode[tmpchr];
    ln++;
    tmpnum = (long)(tmpnum / base);
  }
  
  for (i = len - 1; i >= 0; i--)
  {
    retstring.concat(tmp[i]);
  }
  
  return retstring;
}

unsigned long unstrzero(String hexstr, int base)   // 將字串轉為無符號長整數
{
  int len = hexstr.length();
  unsigned int i = 0;
  unsigned int tmp = 0;
  unsigned int tmp1 = 0;
  unsigned long tmpnum = 0;
  String hexcode = "0123456789ABCDEF";
  
  for (i = 0; i < len; i++)
  {
    hexstr.toUpperCase();
    tmp = hexstr.charAt(i);
    tmp1 = hexcode.indexOf(tmp);
    tmpnum += tmp1 * POW(base, (len - i - 1));
  }
  
  return tmpnum;
}

String print2HEX(int number)   // 將數字轉為兩位數 16 進位字串
{
  String ttt;
  if (number >= 0 && number < 16)
  {
    ttt = "0" + String(number, HEX);
  }
  else
  {
    ttt = String(number, HEX);
  }
  return ttt;
}

String GetMacAddress()    // 取得並回傳 MAC 位址
{
  String Tmp = "";
  byte mac[6];
  
  WiFi.macAddress(mac);   // 取得 MAC 位址
  for (int i = 0; i < 6; i++)
  {
    Tmp.concat(print2HEX(mac[i]));
  }
  
  Tmp.toUpperCase();
  return Tmp;
}

void ShowMAC()  // 在通訊埠印出 MAC 位址
{
  Serial.print("MAC Address:(");  // 印出 "MAC Address:("
  Serial.print(MacData);   // 印出 MAC 位址
  Serial.print(")\n");    // 印出 ")\n"
}

String IpAddress2String(const IPAddress& ipAddress)   // 將 IPAddress 轉為字串
{
  return String(ipAddress[0]) + "." + \
         String(ipAddress[1]) + "." + \
         String(ipAddress[2]) + "." + \
         String(ipAddress[3]);
}

String chrtoString(char *p)   // 將字元指標轉為字串
{
  String tmp;
  char c;
  int count = 0;
  while (count < 100)
  {
    c = *p;
    if (c != 0x00)
    {
      tmp.concat(String(c));
    }
    else
    {
      return tmp;
    }
    count++;
    p++;
  }
}

void CopyString2Char(String ss, char *p)   // 將字串複製到字元陣列
{
  if (ss.length() <= 0)
  {
    *p = 0x00;
    return;
  }
  ss.toCharArray(p, ss.length() + 1);
}

boolean CharCompare(char *p, char *q)   // 比較兩個字元指標的內容
{
  boolean flag = false;
  int count = 0;
  int nomatch = 0;
  while (flag < 100)
  {
    if (*(p + count) == 0x00 || *(q + count) == 0x00)
      break;
    if (*(p + count) != *(q + count))
    {
      nomatch++;
    }
    count++;
  }
  return nomatch == 0;
}

String Double2Str(double dd, int decn)   // 將浮點數轉為字串，指定小數位數
{
  int a1 = (int)dd;
  int a3;
  if (decn > 0)
  {    
    double a2 = dd - a1;
    a3 = (int)(a2 * POW(10, decn));
  }
  
  if (decn > 0)
  {
    return String(a1) + "." + String(a3);
  }
  else
  {
    return String(a1);
  }
}
