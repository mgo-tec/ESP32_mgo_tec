/*
  ili9341_spi.h - for Arduino core for the ESP32 ( Use SPI library ).
  Beta version 1.0.7
  ESP32_LCD_ILI9341_SPI library class has been redesigned.
  
The MIT License (MIT)

Copyright (c) 2018 Mgo-tec. All rights reserved.
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Modify Display.cpp of M5stack library.
M5stack library - MIT License
Copyright (c) 2017 M5Stack
*/

#ifndef MGO_TEC_ESP32_BV1_LCD_ILI9341_SPI_H_INCLUDED_
#define MGO_TEC_ESP32_BV1_LCD_ILI9341_SPI_H_INCLUDED_

#include <Arduino.h>
#include <SPI.h>
#include <soc/spi_reg.h>
#include <utility> //use std::swap
#include "html_color_code.h"

// In the ili9341_spi.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

//------------------------------------
class FontParameter
{
private:
  HtmlColorCodeClass mp_hccc;

public:
  FontParameter(){
    Xsize = 1;
    Ysize = 1;
    prev_Xsize = 1;
    prev_Ysize = 1;
    x0 = 0;
    y0 = 0;
    red = 31;
    green = 63;
    blue = 31;
    bg_red = 0;
    bg_green = 0;
    bg_blue = 0;
    //Serial.println(F("FontParameter Constructor called!"));
  };

  ~FontParameter(){
    //Serial.println(F("FontParameter Destructor called!"));
  };

  uint8_t Xsize, Ysize;
  uint8_t prev_Xsize, prev_Ysize;
  int16_t x0, y0;
  uint8_t red, green, blue;
  uint8_t bg_red, bg_green, bg_blue;

  uint8_t dot_msb = 0, dot_lsb = 0;
  uint8_t bg_dot_msb = 0, bg_dot_lsb = 0;

  uint16_t max_disp_x0_x1, max_disp_y0_y1;
  uint8_t txt_width = 0, txt_height = 0;
  uint16_t x_pixel_size, y_pixel_size;
  uint8_t x_padding = 4, y_padding = 4;

  void htmlColorCode( String html_color_code );
  void htmlBgColorCode( String html_color_code );
  void colorRGB255( uint8_t red, uint8_t green, uint8_t blue );
  void bgColorRGB255( uint8_t red, uint8_t green, uint8_t blue );
  //void convHtmlColCodeTo65kCol( String html_color_code, uint8_t &red, uint8_t &green, uint8_t &blue );
};

//------------------------------------
class ScrolleParameter
{
private:

public:
  ScrolleParameter(){
    //Serial.println(F("ScrolleParameter Constructor called!"));
  };

  ~ScrolleParameter(){
    //Serial.println(F("ScrolleParameter Destructor called!"));
  };

  uint16_t max_disp_x0_x1, max_disp_y0_y1;

  int16_t interval = 0;
  uint16_t font_count = 0;
  uint8_t full_or_half = 0; //全角又は半角、半角=1,全角=2
  uint16_t full_or_half_cnt = 0; //全角または半角カウント
  //uint16_t m_scl_X_cnt = 0;
  uint16_t y_cnt = 0;
  uint16_t x_pixel_size,  y_pixel_size;
  uint16_t font_sjis_len = 0;
  uint8_t txt_width = 0, txt_height = 0;
  uint8_t disp_txt_len = 40; //ディスプレイに表示する8x16フォント文字数

  uint32_t scl_last_time = 0;
  uint8_t single_fnt_scl_cnt = 0;
  uint16_t x_scl_send_bytes = {};
  uint16_t y_scl_send_bytes = {};

public:
  uint8_t **heap_array; //スクロール用配列ヒープ領域確保用
  uint16_t heap_array_size1, heap_array_size2;

  bool isHeap_create = false;
};
  
//------------------------------------
class GraphParameter
{
private:

public:
  GraphParameter(){
    //Serial.println(F("ScrolleParameter Constructor called!"));
  };

  ~GraphParameter(){
    //Serial.println(F("ScrolleParameter Destructor called!"));
  };

  boolean isHeap_create = false;
  int16_t frame_x0 = 0;
  int16_t frame_y0 = 0;
  int16_t frame_x1 = 319;
  int16_t frame_y1 = 102;
  int16_t x_data_min = 1;
  int16_t y_data_min = 1;
  int16_t x_data_max = 318;
  int16_t y_data_max = 101;

  uint16_t x_total_points = 317;
  uint16_t y_total_points = 100;

  float max_value = 100.0;
  float min_value = 0.0;
  
  uint8_t red = 31, green = 63, blue = 31;
  uint8_t bg_red = 0, bg_green = 0, bg_blue = 0;

public:
  //heap memory
  float *array_fGraph;

};

//------------------------------------
class ILI9341Spi
{
private: //SPIインターフェース関連変数
  const uint8_t mp_spi_num = 0x3; //VSPI=0x3, HSPI=0x2
  int8_t mp_sck;
  int8_t mp_miso;
  int8_t mp_mosi;
  int8_t mp_rst;
  int8_t mp_ledpin;
  int8_t mp_dc;
  int8_t mp_cs;
  bool mp_isIPS_lcd = false;
  bool mp_useHw_Cs;
  uint32_t mp_Freq;
  HtmlColorCodeClass mp_hccc;

private:
  const uint16_t mp_max_disp_width = 320;
  const uint16_t mp_max_disp_height = 240;

  void spiWriteBlock( uint16_t color, uint32_t repeat );
  void rangeXY( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 );
  void blockSPIfastWrite( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue, uint32_t repeat );
  uint8_t array2newCreate( ScrolleParameter &scl_set, uint16_t ary_size1, uint16_t ary_size2 );

  void scrolleFormSet( FontParameter &font, ScrolleParameter &scl_set, uint8_t txt_width, uint8_t txt_height, uint8_t Xsize, uint8_t Ysize );
  void fontParamMaxClip( FontParameter &font, ScrolleParameter &scl_set );
  void fontParamMaxClip( FontParameter &font );

public: //LCD変数群
  uint16_t m_max_disp_width = mp_max_disp_width;
  uint16_t m_max_disp_height = mp_max_disp_height;
  uint8_t m_max_font8x16_x = m_max_disp_width / 8;
  uint8_t m_max_font8x16_y = m_max_disp_height / 16;
  uint16_t m_max_pix_x1 = m_max_disp_width - 1;
  uint16_t m_max_pix_y1 = m_max_disp_height - 1;

public:
  void ILI9341init( int8_t sck = 18,
                    int8_t miso = -1,
                    int8_t mosi = 23,
                    int8_t cs = 14,
                    int8_t dc = 27,
                    int8_t rst = 33,
                    int8_t led = 32,
                    uint32_t clk = 40000000,
                    bool use_hwcs = false );
  void commandWrite( uint8_t b );
  void dataWrite( uint8_t b );
  void dataWrite16( uint16_t b );
  void dataWrite32( uint32_t b );
  void dataWriteBytes( uint8_t *b, uint32_t b_size );
  void spiSetChange();
  void dispRotation( uint8_t rot );
  void dispInversionOn();
  void dispInversionOff();
  void displayClear();
  void displayClear( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 );
  void drawPixel65kColRGB565Bytes( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *b, uint32_t b_size );
  void drawPixel65kDotColor_sd( uint16_t x0, uint16_t y0, uint16_t DotColor );
  void drawPixel65k3Color_sd( uint16_t x0, uint16_t y0, String html_color_code );
  void drawPixel65k3Color_sd( uint16_t x0, uint16_t y0, uint8_t red, uint8_t green, uint8_t blue );
  //------SPIFFS use-------
  void drawPixel65kDotColor( uint16_t x0, uint16_t y0, uint16_t DotColor );
  void drawPixel65k3Color( uint16_t x0, uint16_t y0, String html_color_code );
  void drawPixel65k3Color( uint16_t x0, uint16_t y0, uint8_t red, uint8_t green, uint8_t blue );
  //-----------------------
  void drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, String html_color_code );
  void drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t red, uint8_t green, uint8_t blue );
  void drawHorizontalLine( int16_t x0, int16_t x1, int16_t y0, String html_color_code );
  void drawHorizontalLine( int16_t x0, int16_t x1, int16_t y0, uint8_t red, uint8_t green, uint8_t blue );
  void drawVerticallLine( int16_t x0, int16_t y0, int16_t y1, String html_color_code );
  void drawVerticallLine( int16_t x0, int16_t y0, int16_t y1, uint8_t red, uint8_t green, uint8_t blue );
  void drawRectangleLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, String html_color_code );
  void drawRectangleLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue );
  void drawRectangleFill( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, String html_color_code );
  void drawRectangleFill( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue );
  void drawRectangleFill255( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue );
  void drawCircleLine( uint16_t x0, uint16_t y0, uint16_t r, String html_color_code );
  void drawCircleLine( uint16_t x0, uint16_t y0, uint16_t r, uint8_t red, uint8_t green, uint8_t blue );
  void drawCircleFill( uint16_t x0, uint16_t y0, uint16_t r, String html_color_code );
  void drawCircleFill( uint16_t x0, uint16_t y0, uint16_t r, uint8_t red, uint8_t green, uint8_t blue );
  void idleModeOff();
  void idleModeOn();
  void brightness( uint8_t brightness );

  //-------------以下フォント表示系関数----------------------------
  uint8_t XscrolleFontArrayInit( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_width, uint8_t Xsize, uint8_t Ysize );
  uint8_t YscrolleFontArrayInit( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_height, uint8_t Xsize, uint8_t Ysize );
  uint8_t XscrolleFontArrayInitMax( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_width, uint8_t Xsize, uint8_t Ysize );
  uint8_t YscrolleFontArrayInitMax( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_height, uint8_t Xsize, uint8_t Ysize );
  void scrolleArrayDelete( ScrolleParameter &scl_set );

  void scrolleFontSetUp( FontParameter &font, ScrolleParameter &scl_set, bool xy_range_set = false ); 
  void scrolleFontColorSet( FontParameter &font );
  void scrolleXYrangeComChange( FontParameter &font, ScrolleParameter &scl_set );

  void scrolle8x16font( FontParameter &font, ScrolleParameter &scl_set, uint16_t *fnt_cnt, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set = true );
  boolean scrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set = true );
  void reverseScrolle8x16font( FontParameter &font, ScrolleParameter &scl_set, uint16_t *fnt_cnt, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set = true );
  boolean reverseScrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set = true );
  boolean Yscrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set = true );
  boolean YdownScrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set = true );
  void display8x16Font( FontParameter &font, uint16_t fontSJ_Length, uint8_t Fnt[][16] );

};

}// namespace mgo_tec_esp32_bv1
#endif

