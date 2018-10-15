/*
  button_select_box.h - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.01
  
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

#ifndef MGO_TEC_ESP32_BV1_BUTTON_SELECT_BOX_H_INCLUDED_
#define MGO_TEC_ESP32_BV1_BUTTON_SELECT_BOX_H_INCLUDED_

#include <Arduino.h>
#include "ESP32_mgo_tec_bV1/Switch/button_switch.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/display_shinonome_fnt.h"

extern mgo_tec_esp32_bv1::ILI9341Spi LCD;

// In the button_select_box.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

//*****************************
class SelectUrl
{
public:
  String name;
  const char *url;
};

//*****************************
class BtnDispSelectBox
{
private:
  DispShinonomeFnt Dsf;

public:

  uint8_t dispBtnSelUrlDef( ButtonSwitch &Btn,
                           SelectUrl Sel_Url[],
                           uint16_t x0,
                           uint16_t y0,
                           uint16_t x1,
                           uint8_t now_num,
                           uint8_t page_max_num,
                           uint8_t max_num );
  uint8_t dispBtnSelUrl( FontParameter &font,
                       ButtonSwitch &Btn,
                       SelectUrl Sel_Url[],
                       uint16_t x0,
                       uint16_t y0,
                       uint16_t x1,
                       uint8_t now_num,
                       uint8_t page_max_num,
                       uint8_t max_num );
  uint8_t dispBtnSelStrDef( ButtonSwitch &Btn,
                           String str[],
                           uint16_t x0,
                           uint16_t y0,
                           uint16_t x1,
                           uint8_t now_num,
                           uint8_t page_max_num,
                           uint8_t max_num );
  uint8_t dispBtnSelStr( FontParameter &font,
                       ButtonSwitch &Btn,
                       String str[],
                       uint16_t x0,
                       uint16_t y0,
                       uint16_t x1,
                       uint8_t now_num,
                       uint8_t page_max_num,
                       uint8_t max_num );
  void dispSelectStr( FontParameter &font,
                       String str[],
                       uint16_t x0,
                       uint16_t y0,
                       uint16_t x1,
                       uint8_t now_num,
                       int8_t *prev_page,
                       int8_t page_max_num,
                       uint8_t max_num );
  void dispPagePartsStr( FontParameter &font,
                         String str[],
                         uint16_t x0,
                         uint16_t y0,
                         uint16_t x1,
                         uint8_t start_num,
                         uint8_t end_num,
                         uint8_t page_max_num );

};

}// namespace mgo_tec_esp32_bv1

#endif
