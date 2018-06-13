/*
  watch.h - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
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

#ifndef _MGO_TEC_ESP32_ILI9341_SD_WATCH_H_INCLUDED
#define _MGO_TEC_ESP32_ILI9341_SD_WATCH_H_INCLUDED

#include <Arduino.h>
#include "TimeLib.h" //Use Arduino time library ver1.5-
#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h" //beta ver 1.22-
#include "ESP32_mgo_tec_bV1/Web/webget.h"

extern mgo_tec_esp32_bv1::ILI9341Spi LCD;
extern mgo_tec_esp32_bv1::SdShinonomeFont SFR;

// In the watch.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.


class ILI9341Watch
{
static const uint8_t MAX_TXT_NUM = 6; //スクロール表示最大数(時計数字６桁）

private:
  WebGetClass EWG;

  int16_t mp_prev_ymd = -1;
  int8_t mp_prev_hour1 = -1, mp_prev_hour2 = -1;
  int8_t mp_now_hour1 = -1, mp_now_hour2 = -1;
  int8_t mp_prev_min1 = -1, mp_prev_min2 = -1;
  int8_t mp_now_min1 = -1, mp_now_min2 = -1;
  int8_t mp_prev_sec1 = -1, mp_prev_sec2 = -1;
  int8_t mp_now_sec1 = -1, mp_now_sec2 = -1;

  uint32_t mp_font_change_last_time = 0;
  bool mp_changeFont = false;

  int32_t mp_scl_last_time = 0;
  bool mp_startScrolle[ MAX_TXT_NUM ] = {};
  bool m_changeYMD = false;
  uint32_t mp_ntp_get_last_time = 0;
  uint32_t mp_colon_last_time = 0;

  uint8_t mp_watch_sjis_buf[ MAX_TXT_NUM ][ 20 ] = {}; //Shift_JIS文字コード格納
  uint16_t mp_watch_sjis_len[ MAX_TXT_NUM ] = {}; //文字列のShift_JISコード長
  uint8_t mp_watch_font_buf[ MAX_TXT_NUM ][2][16] = {}; //16x16フォント全角１文字格納バッファ
  uint16_t mp_watch_font_count[ MAX_TXT_NUM ] = {}; //半角フォント１文字スクロールカウント

public:
  enum NtpState { MsgReset, Connecting, ConnectOK, ConnectFailed }
    NtpStatus = MsgReset;

  int ntp_msg_status = 0;

  FontParameter
    font[ MAX_TXT_NUM ],
    colon1_font,
    colon2_font,
    ymd_font;

  ScrolleParameter scllole[ MAX_TXT_NUM ];

  //時刻コロンフォント表示用変数
  bool m_onColon_disp = false;
  //-----NTPサーバー時刻取得　引数初期化------------
  bool m_isNtp_first_get = true;

public:
  void watchFontSetup( uint16_t x0, uint16_t y0, uint8_t Xsize, uint8_t Ysize );
  void watchFontRead();
  void scrolleWatch();
  void displayColon(uint16_t colon1_sj_len, uint16_t colon2_sj_len, uint8_t colon1_buf[][ 16 ], uint8_t colon2_buf[][ 16 ]);
  void displayColon2();
  void getNTP();
  void getNTP2(uint32_t get_interval);
  void getNTPserverSel( int timezone, const char *ntp_server_name );
  void watchStrFontConv(uint8_t num, String str);
  void displayYMDW();
  void watchDispReset();

};

}// namespace mgo_tec_esp32_bv1

#endif
