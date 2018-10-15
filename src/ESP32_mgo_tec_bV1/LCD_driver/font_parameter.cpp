/*
  font_parameter.cpp - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.00
  
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

#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"

// In the font_parameter.cpp file
namespace mgo_tec_esp32_bv1 {
// Definition of functions is within scope of the namespace.

//********** Font HTML Color Code str to 65k color value *************
void FontParameter::htmlColorCode( String html_color_code ){
  mp_hccc.convHtmlColCodeTo65kCol(
    html_color_code,
    FontParameter::red,
    FontParameter::green,
    FontParameter::blue );
}
//********** Font HTML Background-Color Code str to 65k color value *************
void FontParameter::htmlBgColorCode( String html_color_code ){
  mp_hccc.convHtmlColCodeTo65kCol(
    html_color_code,
    FontParameter::bg_red,
    FontParameter::bg_green,
    FontParameter::bg_blue );
}

}  // namespace mynamespace
