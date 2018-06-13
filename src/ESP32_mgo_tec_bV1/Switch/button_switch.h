/*
  button_switch.h - for Arduino core for the ESP32.
  Beta version 1.0
  
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

#ifndef _MGO_TEC_ESP32_BUTTON_SWITCH_H_INCLUDED
#define _MGO_TEC_ESP32_BUTTON_SWITCH_H_INCLUDED

#include <Arduino.h>

// In the button_switch.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

class ButtonSwitch
{
public:
  enum BtnStatus { Release, OnStart, MomentPress, LongPress, ContPress }
    ButtonStatus = Release;

  void buttonAction( const uint8_t gpio, boolean continuous_set, uint32_t chatter_time, uint32_t long_press_time );

private:
  BtnStatus mp_ButtonStatus = Release;
  uint32_t mp_ButtonStartTime = 0;

  uint32_t timeMesure( uint32_t st_time );
};

}// namespace mgo_tec_esp32_bv1

#endif
