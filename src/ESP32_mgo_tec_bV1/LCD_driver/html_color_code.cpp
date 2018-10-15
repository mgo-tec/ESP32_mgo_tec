/*
  html_color_code.cpp - for Arduino core for the ESP32.
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

#include "ESP32_mgo_tec_bV1/LCD_driver/html_color_code.h"

// In the html_color_code.cpp file
namespace mgo_tec_esp32_bv1 {
// Definition of functions is within scope of the namespace.

//********** Font HTML Color Code str to 65k color value *************
void HtmlColorCodeClass::convHtmlColCodeTo65kCol( String html_color_code, uint8_t &red, uint8_t &green, uint8_t &blue ){
  if( html_color_code.indexOf( "#" ) == 0 ){
    String red_str = html_color_code.substring( 1, 3 );
    String green_str = html_color_code.substring( 3, 5 );
    String blue_str = html_color_code.substring( 5, 7 );
    uint8_t red_value = strtol( red_str.c_str(), NULL, 16 );
    uint8_t green_value = strtol( green_str.c_str(), NULL, 16 );
    uint8_t blue_value = strtol( blue_str.c_str(), NULL, 16 );
    //red:max=31, green:max=63, blue:max=31
    red = (uint8_t)floor( (double)red_value / 8.0 );
    green = (uint8_t)floor( (double)green_value / 4.0 );
    blue = (uint8_t)floor( (double)blue_value / 8.0 );
    return;
  }else if( html_color_code == "white" ){
    red = 31, green = 63, blue = 31;
  }else if( html_color_code == "black" ){
    red = 0, green = 0, blue = 0;
  }else if( html_color_code == "red" ){
    red = 31, green = 0, blue = 0;
  }else if( html_color_code == "green" ){
    red = 0, green = 63, blue = 0;
  }else if( html_color_code == "blue" ){
    red = 0, green = 0, blue = 31;
  }else if( html_color_code == "yellow" ){
    red = 31, green = 63, blue = 0;
  }else if( html_color_code == "cyan" ){
    red = 0, green = 63, blue = 31;
  }else if( html_color_code == "magenta" ){
    red = 31, green = 0, blue = 31;
  }
}

}  // namespace mynamespace
