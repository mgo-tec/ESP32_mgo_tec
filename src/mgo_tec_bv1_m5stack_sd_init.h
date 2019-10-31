/*
  mgo_tec_bv1_m5stack_sd_init.h - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.1
  
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

#ifndef MGO_TEC_BV1_M5STACK_SD_INIT_H_INCLUDED_
#define MGO_TEC_BV1_M5STACK_SD_INIT_H_INCLUDED_

#include "mgo_tec_bv1_esp32_sd_def.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/watch.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/message_window.h"
#include "ESP32_mgo_tec_bV1/Switch/button_switch.h"

extern mgo_tec_esp32_bv1::ILI9341Spi LCD;
extern mgo_tec_esp32_bv1::SdShinonomeFont SFR;

// In the mgo_tec_bv1_m5stack_sd_init.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

class MgotecM5stackSdInit
{
private:
  const uint8_t buttonA_GPIO = 39;
  const uint8_t buttonB_GPIO = 38;
  const uint8_t buttonC_GPIO = 37;

public:
  ILI9341Watch watch;
  MessageWindow msg, wifi_msg;
  ButtonSwitch btnA, btnB, btnC;

  void init( const char* utf8sjis_file,
              const char* shino_half_font_file,
              const char* shino_full_font_file );
  void init();
  void init_button();

};

}// namespace mgo_tec_esp32_bv1

#endif
