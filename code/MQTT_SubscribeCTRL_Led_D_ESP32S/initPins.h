#define RelayPin 4 // 繼電器控制腳位定義
#define terminator '\n' // 指令結束符號定義
int ccmd = -1 ; // 命令變數初始化為 -1
String cmdstr ; // 用於儲存接收到的命令字串

#include <WiFi.h>   // 使用 WiFi 函式庫
#include <WiFiClient.h>   // 使用 WiFi 用戶端函式庫
#include <WiFiMulti.h>    // 使用多熱點網路函式庫

WiFiMulti wifiMulti;    // 建立多熱點連線物件

#define LedPin 15 // LED 控制腳位定義
#define ON HIGH  // 定義 LED 開啟狀態
#define OFF LOW  // 定義 LED 關閉狀態

// 將 IPAddress 轉換為字串函式
String IpAddress2String(const IPAddress& ipAddress) ;

IPAddress ip ;    // 儲存網路卡取得的 IP 位址原始型態
String IPData ;   // 儲存網路卡取得的 IP 位址字串
String APname ;   // 儲存網路熱點名稱
String MacData ;  // 儲存網路卡的 MAC 地址
long rssi ;   // 儲存網路訊號強度
int status = WL_IDLE_STATUS;  // 儲存網路連線狀態

void initWiFi()   // 網路連線初始化函式
{
  // 加入 WiFi 熱點資訊
  wifiMulti.addAP("NCNUIOT", "12345678");  // 加入第一組熱點
  wifiMulti.addAP("NCNUIOT2", "12345678"); // 加入第二組熱點
  wifiMulti.addAP("NUKIOT", "iot12345");   // 加入第三組熱點

  // 開始連線到 WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to "); // 印出連線提示訊息
  wifiMulti.run();  // 啟動多熱點連線

  while (WiFi.status() != WL_CONNECTED)  // 檢查是否連線成功
  {
    // 連線過程中印出 '.' 表示進度
    Serial.print(".");
    delay(500) ;  // 延遲 500 毫秒
    wifiMulti.run();  // 持續執行多熱點連線
  }

  // 顯示連線成功資訊
  Serial.println("WiFi connected");
  Serial.print("AP Name: ");
  APname = WiFi.SSID();
  Serial.println(APname); // 印出連線的熱點名稱
  Serial.print("IP address: ");
  ip = WiFi.localIP();
  IPData = IpAddress2String(ip) ;
  Serial.println(IPData); // 印出取得的 IP 位址
}

void ShowInternet()   // 顯示網路連線資訊
{
  Serial.print("MAC:") ;
  Serial.print(MacData) ;
  Serial.print("\n") ;
  Serial.print("SSID:") ;
  Serial.print(APname) ;
  Serial.print("\n") ;
  Serial.print("IP:") ;
  Serial.print(IPData) ;
  Serial.print("\n") ;
}

long POW(long num, int expo)  // 計算 num 的 expo 次方
{
  long tmp =1 ;
  if (expo > 0)
  { 
        for(int i = 0 ; i< expo ; i++)
          tmp = tmp * num ;
         return tmp ; 
  } 
  else
  {
   return tmp ; 
  }
}

String SPACE(int sp)  // 回傳指定長度的空白字串
{
    String tmp = "" ;
    for (int i = 0 ; i < sp; i++)
      {
          tmp.concat(' ')  ;
      }
    return tmp ;
}

String strzero(long num, int len, int base)  // 將數值轉為指定進位並補 0
{
  String retstring = String("");
  int ln = 1 ;
    int i = 0 ; 
    char tmp[10] ;
    long tmpnum = num ;
    int tmpchr = 0 ;
    char hexcode[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
    while (ln <= len)
    {
        tmpchr = (int)(tmpnum % base) ;
        tmp[ln-1] = hexcode[tmpchr] ;
        ln++ ;
         tmpnum = (long)(tmpnum/base) ;
    }
    for (i = len-1; i >= 0 ; i --)
      {
          retstring.concat(tmp[i]);
      }
  return retstring;
}

unsigned long unstrzero(String hexstr, int base)  // 將字串轉換為數值
{
  int len = hexstr.length() ;
  unsigned long tmpnum = 0 ;
  String hexcode = String("0123456789ABCDEF") ;

  for (int i = 0 ; i < len ; i++)
  {
      hexstr.toUpperCase() ;
      int tmp = hexstr.charAt(i) ;
      int tmp1 = hexcode.indexOf(tmp) ;
      tmpnum = tmpnum + tmp1 * POW(base, (len - i - 1));
  }
  return tmpnum;
}

String  print2HEX(int number)  // 將數字轉換為 2 位元 16 進位格式
{
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number,HEX);
  }
  else
  {
      ttt = String(number,HEX);
  }
  return ttt ;
}

String GetMacAddress()    // 取得網路卡的 MAC 位址
{
  String Tmp = "" ;
  byte mac[6];
  WiFi.macAddress(mac);
  for (int i=0; i<6; i++)
    {
        Tmp.concat(print2HEX(mac[i])) ;
    }
    Tmp.toUpperCase() ;
  return Tmp ;
}

void ShowMAC()  // 顯示 MAC 位址於串列埠
{
  Serial.print("MAC Address:(");
  Serial.print(MacData) ;
  Serial.print(")\n");
}

String IpAddress2String(const IPAddress& ipAddress)
{
  // 將 IPAddress 型態轉換為字串格式，並以 "." 分隔各段
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

String chrtoString(char *p)
{
    // 將字元陣列轉換為 String 字串
    String tmp ;
    char c ;
    int count = 0 ;
    while (count <100)
    {
        c= *p ;
        if (c != 0x00)
          {
            tmp.concat(String(c)) ; // 逐字元加入 String 字串
          }
          else
          {
              return tmp ; // 若遇到結束符號則返回結果
          }
       count++ ;
       p++;
    }
}

void CopyString2Char(String ss, char *p)
{
  // 將 String 字串複製到 char 陣列
  if (ss.length() <=0)
      {
           *p =  0x00 ;
          return ;
      }
    ss.toCharArray(p, ss.length()+1) ;
}

String constchar2String(char *p)
{
  // 將 char 陣列轉換為 String 字串
  int i=0 ;
  String tmp ="";
  while (true)
  {
    if (*(p+i) == '\0') // 檢查結束符號
      return tmp ;
    tmp.concat(*(p+i)) ;
  }
}

boolean CharCompare(char *p, char *q)
{
  // 比較兩個 char 陣列內容是否相同
  boolean flag = false ;
  int count = 0 ;
  int nomatch = 0 ;
  while (flag <100)
  {
      if (*(p+count) == 0x00 or *(q+count) == 0x00)
        break ;
      if (*(p+count) != *(q+count) )
          {
             nomatch ++ ; 
          }
         count++ ; 
  }
 if (nomatch >0)
  {
    return false ; // 若有不匹配則返回 false
  }
  else
  {
    return true ; // 若匹配則返回 true
  }
}

String Double2Str(double dd,int decn)
{
    // 將浮點數轉換為字串格式，指定小數點位數
    int a1 = (int)dd ;
    int a3 ;
    if (decn >0)
    {    
        double a2 = dd - a1 ;
        a3 = (int)(a2 * (10^decn)); // 計算小數位數
    }
    if (decn >0)
    {
        return String(a1)+"."+ String(a3) ;
    }
    else
    {
      return String(a1) ; // 無小數位數時只返回整數部分
    }
}

void LedOFF()
{
  // 關閉 LED
  digitalWrite(LedPin,OFF) ;
}

void LedON()
{
  // 開啟 LED
  digitalWrite(LedPin,ON) ;
}

void LedTwinkle()
{
  // 讓 LED 閃爍 5 次
  for(int i = 0;i<5; i++)
    {
      LedON();
      sleep(10); // 開啟 10 毫秒
      LedOFF();
      sleep(10); // 關閉 10 毫秒
    }
}

void initGPIO()
{
   // 初始化 GPIO 並將 LED 設為輸出
   pinMode(LedPin,OUTPUT) ;
   LedOFF(); // 初始設為關閉狀態
}
