/*
  message_window.h - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.21
  
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

#ifndef _MGO_TEC_ESP32_ILI9341_SD_MESSAGEWINDOW_H_INCLUDED
#define _MGO_TEC_ESP32_ILI9341_SD_MESSAGEWINDOW_H_INCLUDED

#include <Arduino.h>
#include "TimeLib.h" //Arduino time library ver1.5
#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h"
#include "ESP32_mgo_tec_bV1/Web/webget.h"

extern mgo_tec_esp32_bv1::ILI9341Spi LCD;
extern mgo_tec_esp32_bv1::SdShinonomeFont SFR;

// In the message_window.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

class MessageWindow
{
private:
  

public:
  enum WifiMsgStatus { WifiMsgReset, WifiConnecting , WifiConnected, WifiFailed }
    WifiStatus = WifiMsgReset;

  enum WebGetMsgStatus { MsgReset, Connecting, ConnectOK, ConnectFailed }
    WebGetStatus = MsgReset;

  FontParameter font;

  int16_t m_prev_msg_num = -1;
  uint8_t m_size = 1;
  uint8_t m_txt_length = 38;
  uint16_t m_x0 = 0;
  uint16_t m_y0 = 0;
  uint8_t m_padding = 8;

  uint8_t m_line_red = 31;
  uint8_t m_line_green = 63;
  uint8_t m_line_blue = 31;
  uint8_t m_red = 31;
  uint8_t m_green = 63;
  uint8_t m_blue = 31;
  uint8_t m_bg_red = 0;
  uint8_t m_bg_green = 0;
  uint8_t m_bg_blue = 0;

  void dispWifiStatusMsg();
  void dispWifiStatusMsg2();
  void dispWifiStatusMsgShort();
  void dispWebGetStatusMsgShort( int &web_status, String str );
  void dispWebGetStatusMsgLong( uint8_t &web_status, String str );
  //--------白黒反転------------
  void dispWifiStatusMsgRev();
  void dispWifiStatusMsgShortRev();
  void dispWebGetStatusMsgShortRev( int &web_status, String str );
  void dispWebGetStatusMsgLongRev( uint8_t &web_status, String str );
  //---------------------------
  void dispMsgWindow( int16_t msg_num, String str );
  void clearMsgWindow( int16_t msg_num );

};

}// namespace mgo_tec_esp32_bv1

#endif
