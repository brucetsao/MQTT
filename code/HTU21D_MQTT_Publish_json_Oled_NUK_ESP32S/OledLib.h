#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
// https://www.arduinolibraries.info/libraries/u8g2
//--------------------
void _update();   // 更新螢幕
void _updateBuffer();   // 更新圖型記憶體內容到畫面
void _setI2CAddress(int adr);   // 設定螢幕的I2C 位址
void _initDisplay();   // 初始化螢幕的必用函數
void initOLED();   // 啟動 OLED 顯示器
void _setCursor(int x, int y);   // 設定游標位置
void _setDrawColor(int color);   // 設定繪製顏色
int _getWidth();   // 取得螢幕寬度
int _getHeight();  // 取得螢幕高度
int _getMaxCharWidth();  // 取得字型的最大寬度
int _getMaxCharHeight(); // 取得字型的最大高度
int _getCharWidth(); // 取得字型的寬度
int _getUTF8Width(); // 取得 UTF8 的寬度
void _clearDisplay();  // 清除畫面
void _setContrast(int t);    // 設定對比度
void _clearOled(); // 清除 OLED 顯示器
void _clearBuffer();   // 清除圖型記憶體
void _Box(int x, int y, int w,int h);  // 畫方形
void _Circle(int x, int y, int r);   // 畫圓形
void _Ellipse(int x, int y, int rx,int ry);    // 畫橢圓形
void _FilledEllipse(int x, int y, int rx,int ry);    // 畫填色橢圓形
void _Triangle(int x1, int y1, int x2, int y2,int x3, int y3);   // 畫三角形
void _HLine(int x, int y, int l);  // 畫水平線
void _VLine(int x, int y, int l);    // 畫垂直線
void _Line(int x1, int y1, int x2, int y2);    // 畫線
void _Point(int x, int y);   // 畫點
void _setFont(const uint8_t *font_8x8);    // 設定字型
void _setFontDirection(int dir);     // 設定字型方向
void _setFontMode(int is_transparent);   // 設定字型透明度
void _TPrint(int x, int y,  const char *ss);   // 指定位置輸出文字
void _TextPrint(int x, int y,  const char *ss, const uint8_t *font_8x8);   // 指定位置輸出文字
void _UTF8Print(int x, int y,  const char *ss, const uint8_t *font_8x8);   // 指定位置輸出 UTF8 文字
void _Print(const char *ss);   // 不指定位置輸出文字

//--------------
// 字型文件參考網址：https://github.com/olikraus/u8g2/wiki/u8g2reference
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

void _update()   
{
  u8g2.updateDisplay();
}

void _updateBuffer()    
{
  u8g2.sendBuffer();
}

void _setI2CAddress(int adr)   
{
  u8g2.setI2CAddress(adr);
}

void _initDisplay()   
{
  u8g2.initDisplay();
}

void initOLED()   // 啟動 OLED 顯示器
{
    if (!u8g2.begin())   // 如果 OLED 顯示器未啟動成功
    {
      Serial.println("Couldn't find OLED!");    // 印出 "找不到 OLED 顯示器！"
      while (1);  // 停在此處
    }
    Serial.println("init OLED successful!");   // 顯示 OLED 初始化成功
    _clearBuffer();   // 清除之前的畫圖記憶體
    _setFont(u8g2_font_logisoso16_tf);  // 設定字型
}

void _setCursor(int x, int y)
{
   u8g2.setCursor(x, y);  
}

void _setDrawColor(int color)
{
  // 設定顏色：0 (背景)，1 (前景)，2 (XOR)
  u8g2.setDrawColor(color);  
}

int _getWidth()
{
   return u8g2.getDisplayWidth();  
}

int _getHeight()
{
   return u8g2.getDisplayHeight();  
}

int _getMaxCharWidth()
{
   return u8g2.getMaxCharWidth();  
}

int _getMaxCharHeight()
{
   return u8g2.getMaxCharHeight();  
}

int _getCharWidth()
{
   return u8g2.getStrWidth(" ");  
}

int _getUTF8Width()
{
   return u8g2.getUTF8Width(" ");  
}

void _clearDisplay()
{
  u8g2.clearDisplay();        
  u8g2.sendBuffer();         // 將記憶體資料傳送到顯示器
}

void _setContrast(int t)
{
  u8g2.setContrast(t);         // 設定對比
}

void _clearOled()
{
  u8g2.clear();         
  u8g2.sendBuffer();         // 將記憶體資料傳送到顯示器
}

void _clearBuffer()
{
  u8g2.clearBuffer();         
  u8g2.sendBuffer();  // 將清除的畫面資料傳送到顯示器
}

void _Box(int x, int y, int w, int h)
{
  u8g2.drawBox(x, y, w, h);   // 畫方形     
}

void _Circle(int x, int y, int r)
{
  u8g2.drawCircle(x, y, r, U8G2_DRAW_ALL);   // 畫完整圓形
}

void _Ellipse(int x, int y, int rx, int ry)
{
  u8g2.drawEllipse(x, y, rx, ry, U8G2_DRAW_ALL);   // 畫橢圓形
}

void _FilledEllipse(int x, int y, int rx, int ry)
{
  u8g2.drawFilledEllipse(x, y, rx, ry, U8G2_DRAW_ALL);   // 畫填滿橢圓形
}

void _Triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
  u8g2.drawTriangle(x1, y1, x2, y2, x3, y3);   // 畫三角形
}

void _HLine(int x, int y, int l)
{
  u8g2.drawHLine(x, y, l);   // 畫水平線
}

void _VLine(int x, int y, int l)
{
  u8g2.drawVLine(x, y, l);   // 畫垂直線
}

void _Line(int x1, int y1, int x2, int y2)
{
  u8g2.drawLine(x1, y1, x2, y2);   // 畫直線
}

void _Point(int x, int y)
{
  u8g2.drawPixel(x, y);   // 畫單一像素點
}

void _setFont(const uint8_t *font_8x8)
{
   u8g2.setFont(font_8x8);   // 設定字型
}

void _setFontDirection(int dir)
{
  u8g2.setFontDirection(dir);   // 設定字型方向：0-左到右, 1-上到下, 2-右到左, 3-下到上
}

void _setFontMode(int is_transparent)
{
  u8g2.setFontMode(is_transparent);   // 設定字型透明模式，1 為透明
}

void _TPrint(int x, int y,  const char *ss)
{
  u8g2.drawStr(x, y, ss);   // 在指定位置輸出字串
}

void _TextPrint(int x, int y,  const char *ss, const uint8_t *font_8x8)
{
  u8g2.setFont(font_8x8);  
  u8g2.drawStr(x, y, ss);   // 在指定位置輸出文字
}

void _UTF8Print(int x, int y,  const char *ss, const uint8_t *font_8x8)
{
  u8g2.setFont(font_8x8);  
  u8g2.drawStr(x, y, ss);   // 在指定位置輸出 UTF8 編碼文字
}

void _Print(const char *ss)
{
  u8g2.print(ss);   // 使用 print 在當前位置輸出字串
}
