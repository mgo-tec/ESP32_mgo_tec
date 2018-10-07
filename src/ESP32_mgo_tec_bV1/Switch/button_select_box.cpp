/*
  button_select_box.cpp - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.0
  
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
*/

#include "ESP32_mgo_tec_bV1/Switch/button_select_box.h"

// In the button_select_box.cpp file
namespace mgo_tec_esp32_bv1 {
// Definition of functions is within scope of the namespace.

//******* URL セレクトボックス ( default )*******************
uint8_t BtnDispSelectBox::dispBtnSelUrlDef( ButtonSwitch &Btn,
                                       SelectUrl Sel_Url[],
                                       uint16_t x0,
                                       uint16_t y0,
                                       uint16_t x1,
                                       uint8_t now_num,
                                       uint8_t page_max_num,
                                       uint8_t max_num )
{
  String str[ max_num ];
  for( int i = 0; i < max_num; i++ ){
    str[ i ] = Sel_Url[ i ].name;
  }
  return BtnDispSelectBox::dispBtnSelStrDef( Btn, str, x0, y0, x1, now_num, page_max_num, max_num );
}
//************************************************
uint8_t BtnDispSelectBox::dispBtnSelUrl( FontParameter &font,
                                       ButtonSwitch &Btn,
                                       SelectUrl Sel_Url[],
                                       uint16_t x0,
                                       uint16_t y0,
                                       uint16_t x1,
                                       uint8_t now_num,
                                       uint8_t page_max_num,
                                       uint8_t max_num )
{
  String str[ max_num ];
  for( int i = 0; i < max_num; i++ ){
    str[ i ] = Sel_Url[ i ].name;
  }
  return BtnDispSelectBox::dispBtnSelStr( font, Btn, str, x0, y0, x1, now_num, page_max_num, max_num );
}
//********* string セレクトボックス ( default )********************
uint8_t BtnDispSelectBox::dispBtnSelStrDef( ButtonSwitch &Btn,
                                       String str[],
                                       uint16_t x0,
                                       uint16_t y0,
                                       uint16_t x1,
                                       uint8_t now_num,
                                       uint8_t page_max_num,
                                       uint8_t max_num )
{
  FontParameter font;
  font.Xsize = 2;
  font.Ysize = 2;
  font.x_padding = 8;
  font.y_padding = 8;
  font.red = 31, font.green = 63, font.red = 31;
  font.bg_red = 0, font.bg_green = 0, font.bg_red = 0;

  return BtnDispSelectBox::dispBtnSelStr( font, Btn, str, x0, y0, x1, now_num, page_max_num, max_num );
}
//************************************************
uint8_t BtnDispSelectBox::dispBtnSelStr( FontParameter &font,
                                       ButtonSwitch &Btn,
                                       String str[],
                                       uint16_t x0,
                                       uint16_t y0,
                                       uint16_t x1,
                                       uint8_t now_num,
                                       uint8_t page_max_num,
                                       uint8_t max_num )
{
  uint8_t ret_num = now_num;
  int8_t prev_page = -1;

  BtnDispSelectBox::dispSelectStr( font, str, x0, y0, x1, now_num, &prev_page, page_max_num, max_num );

  while(1){
    Btn.buttonAction();
    if( Btn.ButtonStatus == Btn.MomentPress ){
      if( ++now_num >= max_num ) now_num = 0;
      BtnDispSelectBox::dispSelectStr( font, str, x0, y0, x1, now_num, &prev_page, page_max_num, max_num );
      ret_num = now_num;
    }else if( Btn.ButtonStatus == Btn.ContPress ){
      break;
    }
    delay(1); //This is absolutely required.
  }
  return ret_num;
}
//************************************************
void BtnDispSelectBox::dispSelectStr( FontParameter &font,
                                     String str[],
                                     uint16_t x0,
                                     uint16_t y0,
                                     uint16_t x1,
                                     uint8_t now_num,
                                     int8_t *prev_page,
                                     int8_t page_max_num,
                                     uint8_t max_num )
{
  int8_t prev_num = now_num - 1;
  int8_t now_page = (uint8_t)floor( (double)now_num / (double)page_max_num );
  uint8_t page_start_num = now_page * page_max_num;
  uint8_t page_end_num = page_start_num + page_max_num;
  if( page_end_num > max_num ) page_end_num = max_num;
  uint8_t now_inpage_num = now_num - page_start_num;
  int8_t prev_inpage_num = now_inpage_num - 1;
  font.x0 = x0 + font.x_padding;

  if( *prev_page != now_page ){
    BtnDispSelectBox::dispPagePartsStr( font, str, x0, y0, x1, page_start_num, page_end_num, page_max_num );
    *prev_page = now_page;
  }

  y0 = y0 + font.y_padding;
  uint8_t dummy_red = font.red;
  uint8_t dummy_green = font.green;
  uint8_t dummy_blue = font.blue;
  uint8_t dummy_bg_red = font.bg_red;
  uint8_t dummy_bg_green = font.bg_green;
  uint8_t dummy_bg_blue = font.bg_blue;

  font.y0 = y0 + font.Ysize * 16 * now_inpage_num;
  font.red = dummy_bg_red, font.green = dummy_bg_green, font.blue = dummy_bg_blue;
  font.bg_red = dummy_red, font.bg_green = dummy_green, font.bg_blue = dummy_blue;

  Dsf.dispText( font, str[ now_num ] );

  font.red = dummy_red, font.green = dummy_green, font.blue = dummy_blue;
  font.bg_red = dummy_bg_red, font.bg_green = dummy_bg_green, font.bg_blue = dummy_bg_blue;

  if( prev_inpage_num >= 0 ){
    font.y0 = y0 + font.Ysize * 16 * prev_inpage_num;
    if( prev_num >= 0 ){
      Dsf.dispText( font, str[ prev_num ] );
    }
  }else if( prev_num < 0 ){
    if( page_max_num > max_num ){
      font.y0 = y0 + font.Ysize * 16 * ( max_num - 1 );
      Dsf.dispText( font, str[ max_num - 1 ] );
    }else{
      font.y0 = y0 + font.Ysize * 16 * ( page_max_num - 1 );
      Dsf.dispText( font, str[ page_max_num - 1 ] );
    }
  }
}
//************************************************
void BtnDispSelectBox::dispPagePartsStr( FontParameter &font,
                                         String str[],
                                         uint16_t x0,
                                         uint16_t y0,
                                         uint16_t x1,
                                         uint8_t start_num,
                                         uint8_t end_num,
                                         uint8_t page_max_num )
{
  uint16_t y1 = y0 + page_max_num * font.Ysize * 16 + font.y_padding * 2;

  LCD.drawRectangleFill( x0, y0, x1, y1 , 0, 0, 0 );
  LCD.drawRectangleLine( x0, y0, x1, y1 , 31, 63, 31 );

  y0 = y0 + font.y_padding;
  uint8_t max_value = end_num - start_num;

  for( int i = 0; i < max_value; i++ ){
    font.y0 = y0 + font.Ysize * 16 * i;
    Dsf.dispText( font, str[ start_num + i ] );
  }
}

}  // namespace mynamespace
