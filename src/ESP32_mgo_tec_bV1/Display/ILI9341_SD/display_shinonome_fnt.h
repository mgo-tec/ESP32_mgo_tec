/*
  display_shinonome_fnt.h - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.2
  
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

#ifndef MGO_TEC_ESP32_BV1_ILI9341_SD_DISPSHINONOMEFNT_H_INCLUDED_
#define MGO_TEC_ESP32_BV1_ILI9341_SD_DISPSHINONOMEFNT_H_INCLUDED_

#include <Arduino.h>
#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h" //beta ver 1.22-

extern mgo_tec_esp32_bv1::ILI9341Spi LCD;
extern mgo_tec_esp32_bv1::SdShinonomeFont SFR;

// In the display_shinonome_fnt.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

//*****************************
class DispShinonomeFnt
{
private:
  boolean mp_isHeap_create = false;
  uint16_t mp_heap_array_size;
  uint8_t *mp_heap_single_array;
  uint8_t mp_font_buf[2][16] = {};

private:
  uint8_t createNewSingleArray( uint16_t ary_size );
  void deleteSingleArray();

public:
  String m_new_str;

public:
  void initScrolle( FontParameter &font, ScrolleParameter &scl_set );
  void setFntHtmlColor( String html_color_code, FontParameter &font );
  void scrolleText( FontParameter &font, ScrolleParameter &scl_set );
  void newSetText( ScrolleParameter &scl_set, String str );
  uint16_t dispText( String str );
  uint16_t dispText( FontParameter &font, String str );

};

}// namespace mgo_tec_esp32_bv1

#endif
