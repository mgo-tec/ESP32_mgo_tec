/*
  yahooj_rss.h - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 2.0.0
  
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

#ifndef MGO_TEC_ESP32_BV1_ILI9341_SD_YAHOOJ_RSS_GET_H_INCLUDED_
#define MGO_TEC_ESP32_BV1_ILI9341_SD_YAHOOJ_RSS_GET_H_INCLUDED_

#include <Arduino.h>
#include <TimeLib.h> //Use Arduino time library ver1.5-
#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/display_shinonome_fnt.h"
#include "ESP32_mgo_tec_bV1/Web/webget.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/message_window.h"

extern mgo_tec_esp32_bv1::ILI9341Spi LCD;
extern mgo_tec_esp32_bv1::SdShinonomeFont SFR;

// In the yahooj_rss.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

//*******************************************************************
class YahooJrssGet
{
private:
  WebGetClass EWG;
  DispShinonomeFnt disp_fnt;

  bool m_isNews_first_get = true;
  bool m_isWeather_first_get = true;
  uint32_t m_news_get_last_time = 0;
  uint32_t m_weather_get_last_time = 0;
  File mp_weather_file;

  void weatherJfontNum( String str, uint8_t wDay, uint8_t Htime, uint8_t Fnum[3], uint8_t col[3][3] );
  
  void JapanWeatherFontNum(uint16_t weather_code, uint8_t wDay, uint8_t Htime, uint8_t (&Fnum)[3], uint8_t (&col)[3][3]);
  
  void myFontReadSd( File F, uint8_t ZorH, uint8_t num, uint8_t (&buf)[2][16] );

public:
  enum NewsState { MsgReset, Connecting, ConnectOK, ConnectFailed }
    NewsStatus = MsgReset,
    WeatherStatus = MsgReset;

  FontParameter news_font, weather_fnt;
  ScrolleParameter news_scl_set;
  MessageWindow news_msg, weather_msg;

  int news_msg_status = 0, weather_msg_status = 0;

  bool m_isNews_get = false;
  bool m_isWeather_get = false;

  uint16_t m_news_font_count = 0; //フォントスクロールカウント（半角１文字単位）
  uint16_t m_news_sj_length = 0; //フォント文字数（半角単位）
  String m_news_str;
  String m_weather_str;

public:
  void initScrolle( FontParameter &font, ScrolleParameter &scl_set );
  void initWeather( const char *weather_font_file );
  void dispYahooJweatherMyFont( FontParameter &weather_font );
  void dispYahooJweatherMyFont( File F, FontParameter &weather_font );

  void dispJapanWeatherMyFont( FontParameter &weather_font );
  void dispJapanWeatherMyFont( File F, FontParameter &weather_font );

  void getYahooJnews( const char *host, const char *target_url );
  void getYahooJnews( const char *host, const uint16_t port, const char *target_url );
  void getYahooJnews( const char *Root_Ca, const char *host, const char *target_url );
  void getYahooJnews( const char *Root_Ca, const char *host, const uint16_t port, const char *target_url );
  void getYahooJnewsRCA( const char *Root_Ca, uint8_t rca_set, const char *host, const char *target_url );
  void getYahooJnewsRCA( const char *Root_Ca, uint8_t rca_set, const char *host, const uint16_t port, const char *target_url );

  void getYahooJnews2( const char *host, const char *target_url, uint32_t get_interval );
  void getYahooJweather( const char *host, const char *target_url );
  void getYahooJweather( const char *Root_Ca, const char *host, const char *target_url );
  void getYahooJweatherRCA( const char *Root_Ca, uint8_t rca_set, const char *host, const char *target_url );

  void getYahooJweather2( const char *host, const char *target_url, uint32_t get_interval );

  void getJpWeatherRCA(
    const char *Root_Ca,
    uint8_t rca_set,
    const char *host,
    String target_url,
    String area_code_str);

  void scrolleYahooJnews( FontParameter &font, ScrolleParameter &scl_set, uint8_t *sj_txt, uint8_t font_buf[][16] );
  void scrolleYahooJnews2( FontParameter &news_font, ScrolleParameter &scl_set );

};

}// namespace mgo_tec_esp32_bv1

#endif
