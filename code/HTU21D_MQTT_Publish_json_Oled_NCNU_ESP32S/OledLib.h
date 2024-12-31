#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
//https://www.arduinolibraries.info/libraries/u8g2
//--------------------
void _update();   //更新螢幕
void _updateBuffer();   //更新圖型記憶體內容到畫面
void _setI2CAddress(int adr);   //設定螢幕的I2C 位址
void _initDisplay();   //begin()起始螢幕必用函數
void initOLED();   //啟動OLED  顯示器
void _setCursor(int x, int y);   //設定目前游標
void _setDrawColor(int color);   //設定目前顏色
int _getWidth();   //取得螢幕寬度
int _getHeight();  //取得螢幕高度
int _getMaxCharWidth();  //取得目前字形最大寬度
int _getMaxCharHeight(); //取得目前字形最大高度
int _getCharWidth(); //取得目前字形寬度
int _getUTF8Width(); //取得目前UTF8寬度
void _clearDisplay();  //清除畫面
void _setContrast(int t);    //設定對比
void _clearOled(); //清除螢幕
void _clearBuffer();   //清除之前畫圖等記憶體
void _Box(int x, int y, int w,int h);  //畫方形
void _Circle(int x, int y, int r);   //畫圓形
void _Ellipse(int x, int y, int rx,int ry);    //畫橢圓形
void _FilledEllipse(int x, int y, int rx,int ry);    //畫填色橢圓形
void _Triangle(int x1, int y1, int x2, int y2,int x3, int y3);   //畫三角形
void _HLine(int x, int y, int l);  //畫水平線
void _VLine(int x, int y, int l);    //畫垂直線
void _Line(int x1, int y1, int x2, int y2);    //畫線
void _Point(int x, int y);   //畫點
void _setFont(const uint8_t *font_8x8);    //設定字形
void _setFontDirection(int dir);     //設定字形方向
void _setFontMode(int is_transparent);   //設定字形透明度
void _TPrint(int x, int y,  const char *ss);   // 指定位置輸出文字
void _TextPrint(int x, int y,  const char *ss, const uint8_t *font_8x8);   // 指定位置輸出文字
void _UTF8Print(int x, int y,  const char *ss, const uint8_t *font_8x8);   // 指定位置輸出UTF8文字
void _Print(const char *ss);   // 不指定位置輸出文字
//--------------
// all font document ref URL:https://github.com/olikraus/u8g2/wiki/u8g2reference
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
void initOLED()   //啟動OLED  顯示器
{
    if (!u8g2.begin())   //如果 OLED  顯示器沒有啟動成功
    {
      Serial.println("Couldn't find OLED!");    //印出 "Couldn't find sensor!"
      //找不到 OLED  顯示器
      while (1);  //永遠死在這
    }
     Serial.println("init OLED successful!");
   // _initDisplay() ;
     _clearBuffer();   //清除之前畫圖等記憶體
   _setFont(u8g2_font_logisoso16_tf);  //設定字形
}
void _setCursor(int x, int y)
{
   u8g2.setCursor(x,y);  
}
void _setDrawColor(int color)
{
/*
Font Mode  Draw Color  Glyph Foreground Color  Glyph Background Color
0: solid  0 0 1
0: solid  1 1 0
0: solid  2 XOR 0
1: transparent  0 0 -
1: transparent  1 1 -
1: transparent  2 XOR -
 */
  
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
  u8g2.sendBuffer();         // transfer internal memory to the display
}
void _setContrast(int t)
{
        
  u8g2.setContrast(t);         // transfer internal memory to the display
}
void _clearOled()
{
  u8g2.clear();         
  u8g2.sendBuffer();         // transfer internal memory to the display
}

void _clearBuffer()
{
  u8g2.clearBuffer();         
  u8g2.sendBuffer();  // Use sendBuffer to transfer the cleared frame buffer to the display.
 }

/*
void drawBitMap(int x, int y, int cnt,int h, static unsigned char bitmap)
{
  
//   x: X-position (left position of the bitmap).
//  y: Y-position (upper position of the bitmap).
//  cnt: Number of bytes of the bitmap in horizontal direction. The width of the bitmap is cnt*8.
//  h: Height of the bitmap.
//  bitmap: Pointer to the start of the bitmap.
   
  u8g2.drawBox(x,y,cnt,h,bitmap);        
 }
*/
void _Box(int x, int y, int w,int h)
{
  /*
   x: X-position (left position of the bitmap).
  y: Y-position (upper position of the bitmap).
  w: width of the bitmap.
  h: Height of the bitmap.
.
   */
  u8g2.drawBox(x,y,w,h);        
 }
void _Circle(int x, int y, int r)
{
  /*
   x: X-position (left position of the bitmap).
  y: Y-position (upper position of the bitmap).
u8g2 : Pointer to the u8g2 structure (C interface only).
x0, y0: Position of the center of the circle.
rad: Defines the size of the circle: Radus = rad.
opt: Selects some or all sections of the circle.
    U8G2_DRAW_UPPER_RIGHT
    U8G2_DRAW_UPPER_LEFT
    U8G2_DRAW_LOWER_LEFT
    U8G2_DRAW_LOWER_RIGHT
    U8G2_DRAW_ALL
.
   */
  u8g2.drawCircle(x,y,r,U8G2_DRAW_ALL);        
 }

void _Ellipse(int x, int y, int rx,int ry)
{
  /*
   x: X-position (left position of the bitmap).
  y: Y-position (upper position of the bitmap).
u8g2 : Pointer to the u8g2 structure (C interface only).
x0, y0: Position of the center of the filled circle.
rx, ry: Defines the size of the ellipse.
opt: Selects some or all sections of the circle.
    U8G2_DRAW_UPPER_RIGHT
    U8G2_DRAW_UPPER_LEFT
    U8G2_DRAW_LOWER_LEFT
    U8G2_DRAW_LOWER_RIGHT
    U8G2_DRAW_ALL
.
   */
  u8g2.drawEllipse(x,y,rx,ry,U8G2_DRAW_ALL);        
 }

void _FilledEllipse(int x, int y, int rx,int ry)
{
  /*
   x: X-position (left position of the bitmap).
  y: Y-position (upper position of the bitmap).
u8g2 : Pointer to the u8g2 structure (C interface only).
x0, y0: Position of the center of the filled circle.
rx, ry: Defines the size of the ellipse.
opt: Selects some or all sections of the circle.
    U8G2_DRAW_UPPER_RIGHT
    U8G2_DRAW_UPPER_LEFT
    U8G2_DRAW_LOWER_LEFT
    U8G2_DRAW_LOWER_RIGHT
    U8G2_DRAW_ALL
.
   */
  u8g2.drawFilledEllipse(x,y,rx,ry,U8G2_DRAW_ALL);  
}
void _Triangle(int x1, int y1, int x2, int y2,int x3, int y3)
{
  /*
   x: X-position (left position of the bitmap).
  y: Y-position (upper position of the bitmap).

   */
  u8g2.drawTriangle(x1,y1,x2,y2,x3,y3);        
 }
void _HLine(int x, int y, int l)
{
  /*
   x: X-position (left position of the bitmap).
  y: Y-position (upper position of the bitmap).

   */
  u8g2.drawHLine(x,y,l);        
 }
void _VLine(int x, int y, int l)
{
  /*
   x: X-position (left position of the bitmap).
  y: Y-position (upper position of the bitmap).
  l:ength of the vertical line.
   */
  u8g2.drawVLine(x,y,l);        
 }
void _Line(int x1, int y1, int x2, int y2)
{
  /*
u8g2 : Pointer to the u8g2 structure (C interface only).
x0: X-position of the first point.
y0: Y-position of the first point.
x1: X-position of the second point.
y1: Y-position of the second point.

   */
  u8g2.drawLine(x1,y1,x2,y2);        
 }

void _Point(int x, int y)
{
  /*
u8g2 : Pointer to the u8g2 structure (C interface only).
x0: X-position of the first point.
y0: Y-position of the first point.
x1: X-position of the second point.
y1: Y-position of the second point.

   */
  u8g2.drawPixel(x,y);        
 }
void _setFont(const uint8_t *font_8x8)
{
   u8g2.setFont(font_8x8);  
}
void _setFontDirection(int dir)
{
  /*
Description: The arguments defines the drawing direction of all strings or glyphs.
Argument  String Rotation Description
0 0 degree  Left to right
1 90 degree Top to down
2 180 degree  Right to left
3 270 degree  Down to top   */
   u8g2.setFontDirection(dir);  
}
void _setFontMode(int is_transparent)
{
  //is_transparent: Enable (1) or disable (0) transparent mode.
   u8g2.setFontMode(is_transparent);  
}

void _TPrint(int x, int y,  const char *ss)
{
  /*
u8g2 : Pointer to the u8g2 structure (C interface only).
x0: X-position of the first point.
y0: Y-position of the first point.
x1: X-position of the second point.
y1: Y-position of the second point.

   */
  u8g2.drawStr(x,y,ss);        
 }

void _TextPrint(int x, int y,  const char *ss, const uint8_t *font_8x8)
{
  /*
u8g2 : Pointer to the u8g2 structure (C interface only).
x0: X-position of the first point.
y0: Y-position of the first point.
x1: X-position of the second point.
y1: Y-position of the second point.

   */
   u8g2.setFont(font_8x8);  
  u8g2.drawStr(x,y,ss);        
 }
void _UTF8Print(int x, int y,  const char *ss, const uint8_t *font_8x8)
{
  /*
u8g2 : Pointer to the u8g2 structure (C interface only).
x0: X-position of the first point.
y0: Y-position of the first point.
x1: X-position of the second point.
y1: Y-position of the second point.
ss: UTF-8 encoded text.
   */
   u8g2.setFont(font_8x8);  
  u8g2.drawStr(x,y,ss);        
 }

void _Print(const char *ss)
{
  /*

ss: UTF-8 encoded text.
   */

  u8g2.print(ss);        
 }
