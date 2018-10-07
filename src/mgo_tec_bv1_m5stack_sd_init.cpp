/*
  mgo_tec_bv1_m5stack_sd_init.cpp - for Arduino core for the ESP32.
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

Use Arduino Time Library ( TimeLib.h )
time.c - low level time and date functions
Copyright (c) Michael Margolis 2009-2014
LGPL ver2.1
https://github.com/PaulStoffregen/Time
*/

#include "mgo_tec_bv1_m5stack_sd_init.h"

// In the mgo_tec_bv1_m5stack_sd_init.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.

//**************************************************************
void MgotecM5stackSdInit::init( const char* utf8sjis_file,
                          const char* shino_half_font_file,
                          const char* shino_full_font_file )
{
  btnA.init( buttonA_GPIO, true, 30, 500 ); //長押し設定 true, チャタリング対策 30ms, 長押し500ms
  btnB.init( buttonB_GPIO, true, 30, 500 ); //長押し設定 true, チャタリング対策 30ms, 長押し500ms
  btnC.init( buttonC_GPIO, true, 30, 500 ); //長押し設定 true, チャタリング対策 30ms, 長押し500ms
  SFR.init( 4, 40000000 ); //sd_cs = 4, sd_frequency = 40000000
  SFR.init3File( utf8sjis_file, shino_half_font_file, shino_full_font_file );
  LCD.ILI9341init();
  LCD.displayClear();
  LCD.brightness( 255 ); //LCD LED Full brightness
}
  

}  // namespace mynamespace
