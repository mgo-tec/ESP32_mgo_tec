/*
  button_switch.cpp - for Arduino core for the ESP32.
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
*/

#include "ESP32_mgo_tec_bV1/Switch/button_switch.h"

// In the button_switch.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.

//****************************************
void ButtonSwitch::init( const uint8_t gpio, boolean continuous_set, uint32_t chatter_time, uint32_t long_press_time ){
  mp_gpio = gpio;
  mp_continuous_set = continuous_set; //長押し設定 true = ON
  mp_chatter_time = chatter_time; //チャタリングタイム
  mp_long_press_time = long_press_time; //長押し時間

  pinMode( gpio, INPUT ); //M5Stack GPIO#37～#39 は内部プルアップ無し
}
//****************************************
void ButtonSwitch::buttonAction(){
  ButtonSwitch::buttonAction( mp_gpio, mp_continuous_set, mp_chatter_time, mp_long_press_time );
}
//****************************************
void ButtonSwitch::buttonAction( const uint8_t gpio, boolean continuous_set, uint32_t chatter_time, uint32_t long_press_time ){
  ButtonStatus = Release;
  switch( digitalRead(gpio) ){
    case 0: // LOW = 0
      switch ( mp_ButtonStatus ){
        case Release:
          mp_ButtonStatus = OnStart;
          mp_ButtonStartTime = millis(); //reset start_time
          break;
        case OnStart:
          if( continuous_set ){             
            if( timeMesure( mp_ButtonStartTime ) > long_press_time ){
              mp_ButtonStatus = ContPress;
            }
          }else{
            if( timeMesure( mp_ButtonStartTime ) > long_press_time ){
              mp_ButtonStatus = LongPress;
              ButtonStatus = LongPress;
            }
          }
          break;
        case ContPress:
          if( timeMesure( mp_ButtonStartTime ) > long_press_time ){
            mp_ButtonStartTime = millis(); //reset start_time
            ButtonStatus = ContPress;
          }
          break;
        default:
          break;
      }
      break;
    case 1: // HIGH = 1
      if( mp_ButtonStatus == OnStart ){
        if( timeMesure( mp_ButtonStartTime ) > chatter_time ){
          ButtonStatus = MomentPress;
        }
      }
      mp_ButtonStatus = Release;
      break;
    default:
      break;
  }
}
//******************************************
uint32_t ButtonSwitch::timeMesure( uint32_t st_time ){
  return millis() - st_time;
}

}  // namespace mynamespace
