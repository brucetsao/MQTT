#define RelayPin 4   // 繼電器控制腳位定義
#define terminator '\n'   // 字符串結束符號
int ccmd = -1 ;   // 命令暫存變數
String cmdstr ;   // 命令字串暫存變數

#include <WiFi.h>   // 使用網路函式庫
#include <WiFiClient.h>   // 使用網路用戶端函式庫
#include <WiFiMulti.h>    // 多熱點網路函式庫

WiFiMulti wifiMulti;    // 產生多熱點連線物件
String IpAddress2String(const IPAddress& ipAddress) ;
IPAddress ip ;    // 網路卡取得IP位址之原始型態之儲存變數
String IPData ;   // 網路卡取得IP位址之儲存變數
String APname ;   // 網路熱點之儲存變數
String MacData ;   // 網路卡取得網路卡編號之儲存變數 
long rssi ;   // 網路連線之訊號強度'之儲存變數
int status = WL_IDLE_STATUS;  // 取得網路狀態之變數

void initWiFi()   // 網路連線，連上熱點
{
  // 加入連線熱點資料
  wifiMulti.addAP("NCNUIOT", "12345678");  // 加入一組熱點
  wifiMulti.addAP("NCNUIOT2", "12345678");  // 加入一組熱點
  wifiMulti.addAP("NUKIOT", "iot12345");  // 加入一組熱點

  // 開始連線至WiFi網路
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");   // 通訊埠印出 "Connecting to "
  wifiMulti.run();  // 多網路熱點設定連線
 while (WiFi.status() != WL_CONNECTED)     // 還沒連線成功
  {
    // 多熱點連線測試
    Serial.print(".");   // 通訊埠印出等待連線中
    delay(500) ;  // 延遲500毫秒
    wifiMulti.run();   // 繼續多網路熱點設定連線
  }
    Serial.println("WiFi connected");   // 通訊埠印出 WiFi 連線成功
    Serial.print("AP Name: ");   // 通訊埠印出 AP名稱:
    APname = WiFi.SSID();
    Serial.println(APname);   // 印出目前連線的熱點名稱
    Serial.print("IP address: ");   // 印出IP位址:
    ip = WiFi.localIP();
    IPData = IpAddress2String(ip) ;
    Serial.println(IPData);   // 印出取得的IP位址
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

long POW(long num, int expo)   // 計算次方
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

String SPACE(int sp)   // 產生指定數量的空白字串
{
    String tmp = "" ;
    for (int i = 0 ; i < sp; i++)
      {
          tmp.concat(' ')  ;
      }
    return tmp ;
}

String strzero(long num, int len, int base)   // 將數字轉換為指定進位與長度的字串
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

unsigned long unstrzero(String hexstr, int base)   // 將字串轉換為數值
{
  String chkstring  ;
  int len = hexstr.length() ;
  unsigned int i = 0 ;
  unsigned int tmp = 0 ;
  unsigned int tmp1 = 0 ;
  unsigned long tmpnum = 0 ;
  String hexcode = String("0123456789ABCDEF") ;
  for (i = 0 ; i < (len ) ; i++)
  {
      hexstr.toUpperCase() ;
      tmp = hexstr.charAt(i) ;
      tmp1 = hexcode.indexOf(tmp) ;
      tmpnum = tmpnum + tmp1* POW(base,(len -i -1) )  ;
  }
  return tmpnum;
}

String  print2HEX(int number) {   // 數字轉為兩位十六進位字串
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

String GetMacAddress()    // 取得網路卡MAC位址
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

void ShowMAC()  // 印出網路卡MAC位址
{
  Serial.print("MAC Address:(");
  Serial.print(MacData) ;
  Serial.print(")\n");
}

String IpAddress2String(const IPAddress& ipAddress)   // IP位址轉字串
{
  return String(ipAddress[0]) + String(".") +
  String(ipAddress[1]) + String(".") +
  String(ipAddress[2]) + String(".") +
  String(ipAddress[3])  ; 
}


String chrtoString(char *p)
{
    String tmp ;
    char c ;
    int count = 0 ;
    while (count <100)
    {
        c= *p ;
        if (c != 0x00)
          {
            tmp.concat(String(c)) ;
          }
          else
          {
              return tmp ;
          }
       count++ ;
       p++;
       
    }
}


void CopyString2Char(String ss, char *p)
{
         //  sprintf(p,"%s",ss) ;

  if (ss.length() <=0)
      {
           *p =  0x00 ;
          return ;
      }
    ss.toCharArray(p, ss.length()+1) ;
   // *(p+ss.length()+1) = 0x00 ;
}

boolean CharCompare(char *p, char *q)
  {
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
        return false ;
      }
      else
      {
        return true ;
      }
      
        
  }


String Double2Str(double dd,int decn)
{
    int a1 = (int)dd ;
    int a3 ;
    if (decn >0)
    {    
        double a2 = dd - a1 ;
        a3 = (int)(a2 * (10^decn));
    }
    if (decn >0)
    {
        return String(a1)+"."+ String(a3) ;
    }
    else
    {
      return String(a1) ;
    }
  
}
