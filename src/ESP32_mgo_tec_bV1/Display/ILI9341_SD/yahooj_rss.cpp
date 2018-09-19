/*
  yahooj_rss.cpp - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.2
  
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

#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/yahooj_rss.h"

// In the yahooj_rss.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.

//*******************************************************
void YahooJrssGet::dispYahooJweatherMyFont( File F, FontParameter &weather_font ){
  if( m_isWeather_get == true ){
    uint8_t today_fnum[3] = {}, tomorrow_fnum[3] = {};
    uint8_t today_col[3][3] ={}, tomorrow_col[3][3] = {};
    uint8_t weather_fnt_buf[2][2][16] = {};

    uint8_t from1 = m_weather_str.indexOf('|', 0);
    uint8_t from2 = m_weather_str.indexOf('|', 2);
    uint8_t to1 = m_weather_str.indexOf('|', 2);
    String w_str1 = m_weather_str.substring(from1, to1);
    String dummy1 = m_weather_str.substring(from2);
    String w_str2 = dummy1.substring(0, dummy1.indexOf('|', 2));
    Serial.print(F("Weather Today = ")); Serial.println(w_str1);
    Serial.print(F("Weather Tomorrow = ")); Serial.println(w_str2);
    //Serial.flush(); //シリアル出力が終わるまで待つ指令は、余分なdelayがかかってしまうので基本的に使わない

    weatherJfontNum(w_str1, 0, hour(), today_fnum, today_col);
    weatherJfontNum(w_str2, 1, hour(), tomorrow_fnum, tomorrow_col);

    uint8_t num;
    //-----------------------------------------------
    FontParameter  index_fnt[ 2 ];
    uint8_t index_buf[ 2 ][ 5 ][ 16 ] = {};
    String index_str[ 2 ] ;
    index_str[0] = "今日";
    index_str[1] = "明日";

    for( num = 0; num < 2; num++ ){
      SFR.convStrToFont(index_str[ num ], index_buf[ num ]);
    }
    num = 0;
    index_fnt[ num ].Xsize = weather_font.Xsize, index_fnt[ num ].Ysize = weather_font.Ysize;
    index_fnt[ num ].x0 = 0, index_fnt[ num ].y0 = weather_font.y0;
    index_fnt[ num ].red = 31, index_fnt[ num ].green = 63, index_fnt[ num ].blue = 31;
    index_fnt[ num ].bg_red = 0, index_fnt[ num ].bg_green = 0, index_fnt[ num ].bg_blue = 0;
    LCD.display8x16Font(index_fnt[ num ], 4, index_buf[ num ]);
    num = 1;
    index_fnt[ num ].Xsize = weather_font.Xsize, index_fnt[ num ].Ysize = weather_font.Ysize;
    index_fnt[ num ].x0 = 160, index_fnt[ num ].y0 = weather_font.y0;
    index_fnt[ num ].red = 0, index_fnt[ num ].green = 63, index_fnt[ num ].blue = 0;
    index_fnt[ num ].bg_red = 0, index_fnt[ num ].bg_green = 0, index_fnt[ num ].bg_blue = 0;
    LCD.display8x16Font(index_fnt[ num ], 4, index_buf[ num ]);

    //----------------------------------------------------
    weather_font.red = 31, weather_font.green = 63, weather_font.blue = 31;
    weather_font.bg_red = 0, weather_font.bg_green = 0, weather_font.bg_blue = 0;

    for(int i=0; i<3; i++){
      if(today_fnum[i] != 0){
        myFontReadSd(F, 2, today_fnum[i], weather_fnt_buf[0]);
      }
      weather_font.x0 = 64 + i * 16 * weather_font.Xsize;
      weather_font.red = today_col[i][0], weather_font.green = today_col[i][1], weather_font.blue = today_col[i][2];
      weather_font.bg_red = 0, weather_font.bg_green = 0, weather_font.bg_blue = 0; 
      LCD.display8x16Font(weather_font, 2, weather_fnt_buf[0]);
    }
    for(int i=0; i<3; i++){
      if(tomorrow_fnum[i] != 0){
        myFontReadSd(F, 2, tomorrow_fnum[i], weather_fnt_buf[1]);
      }
      weather_font.x0 = 224 + i * 16 * weather_font.Xsize;
      weather_font.red = tomorrow_col[i][0], weather_font.green = tomorrow_col[i][1], weather_font.blue = tomorrow_col[i][2];
      LCD.display8x16Font( weather_font, 2, weather_fnt_buf[1] );
    }
    m_isWeather_get = false;
  }
}
//***************** Weather MyFont Number get *************************************************
void YahooJrssGet::weatherJfontNum(String str, uint8_t wDay, uint8_t Htime, uint8_t Fnum[3], uint8_t col[3][3]){
  uint8_t Sunny_red = 31, Sunny_green = 40, Sunny_blue = 0;
  uint8_t Cloudy_red = 29, Cloudy_green = 60, Cloudy_blue = 29;
  uint8_t Rain_red = 0, Rain_green = 0, Rain_blue = 31;
  uint8_t Snow_red = 31, Snow_green = 63, Snow_blue = 31;
  uint8_t Thunder_red = 31, Thunder_green = 63, Thunder_blue = 0;
  uint8_t red = 0, green = 0, blue = 0;

  col[1][0] = 31; col[1][1] = 63; col[1][2] = 31; //矢印

  uint8_t fnt_num = 0;
  bool Single = true;

  if((str.indexOf("時々") >= 0) || (str.indexOf("一時") >= 0)){
    Single = false;
    Fnum[1] = 27;
  }else if(str.indexOf("後") >= 0){
    Single = false;
    Fnum[1] = 28;
  }else if(str.indexOf("時々") < 0 && str.indexOf("後") < 0){
    Single = true;
  }

  if(str.indexOf("晴") == 1){
    if((wDay == 0) && (Htime >= 15)){ //wDay = 0 今日、wDay = 1 明日
      red =  Thunder_red; green = Thunder_green; blue = Thunder_blue;
      fnt_num = 26;
    }else{
      red =  Sunny_red; green = Sunny_green; blue = Sunny_blue;
      fnt_num = 20;
    }
  }else if(str.indexOf("曇") == 1){
    red =  Cloudy_red; green = Cloudy_green; blue = Cloudy_blue;
    fnt_num = 21;
  }else if(str.indexOf("雨") == 1){
    red =  Rain_red; green = Rain_green; blue = Rain_blue;
    fnt_num = 22;
  }else if(str.indexOf("雪") == 1){
    red =  Snow_red; green = Snow_green; blue = Snow_blue;
    fnt_num = 24;
  }else if(str.indexOf("雷") == 1){
    red =  Thunder_red; green = Thunder_green; blue = Thunder_blue;
    fnt_num = 25;
  }else if((str.indexOf("暴風雨") == 1) || (str.indexOf("大雨") == 1)){
    red =  Rain_red; green = Rain_green; blue = Rain_blue;
    fnt_num = 23;
  }

  if(Single == true){
    Fnum[0] = 0;
    Fnum[1] = fnt_num;
    Fnum[2] = 0;
    col[1][0] = red; col[1][1] = green; col[1][2] = blue;
  }else{
    Fnum[0] = fnt_num;
    col[0][0] = red; col[0][1] = green; col[0][2] = blue;
  }

  if(Single == false){
    if(str.indexOf("晴") > 1){
      red =  Sunny_red; green = Sunny_green; blue = Sunny_blue;
      Fnum[2] = 20;
    }else if(str.indexOf("曇") > 1){
      red =  Cloudy_red; green = Cloudy_green; blue = Cloudy_blue;
      Fnum[2] = 21;
    }else if(str.indexOf("雨") > 1){
      red =  Rain_red; green = Rain_green; blue = Rain_blue;
      Fnum[2] = 22;
    }else if(str.indexOf("雪") > 1){
      red =  Snow_red; green = Snow_green; blue = Snow_blue;
      Fnum[2] = 24;
    }else if(str.indexOf("雷") > 1){
      red =  Thunder_red; green = Thunder_green; blue = Thunder_blue;
      Fnum[2] = 25;
    }else if((str.indexOf("暴風雨") > 1) || (str.indexOf("大雨") > 1)){
      red =  Rain_red; green = Rain_green; blue = Rain_blue;
      Fnum[2] = 23;
    }
    col[2][0] = red; col[2][1] = green; col[2][2] = blue;
  }
}
//**************************************
void YahooJrssGet::getYahooJnews( const char *host, const char *target_url ){
  YahooJrssGet::getYahooJnewsRCA( "\0", 0, host, target_url );
}
//**************************************
void YahooJrssGet::getYahooJnews( const char *Root_Ca, const char *host, const char *target_url ){
  YahooJrssGet::getYahooJnewsRCA( Root_Ca, 1, host, target_url );
}
//**************************************
void YahooJrssGet::getYahooJnewsRCA( const char *Root_Ca, uint8_t rca_set, const char *host, const char *target_url ){
  char web_get_time[6];
  sprintf(web_get_time, "%02d:%02d", hour(), minute()); //ゼロを空白で埋める場合は%2dとする
  NewsStatus = Connecting;
  news_msg_status = Connecting;

  String tmp_str = EWG.httpsGet( Root_Ca, rca_set, host, String(target_url), '\n', "</rss>", "<title>", "</title>", "◆ " );

  if( tmp_str.length() < 80 ){
    delay(500); //メッセージウィンドウを正しく表示させるために必要
    m_news_str = tmp_str;
    Serial.printf("Cannot News Getting = %s\r\n", tmp_str.c_str());
    NewsStatus = ConnectFailed;
    news_msg_status = ConnectFailed;
  }else{
    m_news_str = "◆ " + String(web_get_time) + " ";
    m_news_str += tmp_str;
    Serial.printf("News Get = %s\r\n", m_news_str.c_str());
    NewsStatus = ConnectOK;
    news_msg_status = ConnectOK;
  }

  m_isNews_first_get = false;
  m_isNews_get = true;
}
//**************************************
void YahooJrssGet::getYahooJnews2(const char *host, const char *target_url, uint32_t get_interval){
  if( (m_isNews_first_get == true) || ((millis() - m_news_get_last_time) > get_interval) ){
    YahooJrssGet::getYahooJnews( host, target_url );
    m_news_get_last_time = millis();
  }
}
//************** Yahoo RSS 天気予報取得 *************************
void YahooJrssGet::getYahooJweather( const char *host, const char *target_url ){
  YahooJrssGet::getYahooJweatherRCA( "\0", 0, host, target_url );
}
  //************** Yahoo RSS 天気予報取得 *************************
void YahooJrssGet::getYahooJweather( const char *Root_Ca, const char *host, const char *target_url ){
  YahooJrssGet::getYahooJweatherRCA( Root_Ca, 1, host, target_url );
}
//************** Yahoo RSS 天気予報取得 *************************
void YahooJrssGet::getYahooJweatherRCA( const char *Root_Ca, uint8_t rca_set, const char *host, const char *target_url ){
  WeatherStatus = Connecting;
  weather_msg_status = Connecting;

  m_weather_str = EWG.httpsGet( Root_Ca, rca_set, host, String(target_url),  '>', "</rss", "】 ", " - ", "|" );

  Serial.print(F("Weather forecast = ")); Serial.println(m_weather_str);
  if( m_weather_str.indexOf("※") == 0 ){
    delay(500); //メッセージウィンドウを正しく表示させるために必要
    WeatherStatus = ConnectFailed;
    weather_msg_status = ConnectFailed;
  }else{
    WeatherStatus = ConnectOK;
    weather_msg_status = ConnectOK;
  }
  m_isWeather_first_get = false;
  m_isWeather_get = true;
}
//************** Yahoo RSS 天気予報取得 *************************
void YahooJrssGet::getYahooJweather2( const char *host, const char *target_url, uint32_t get_interval ){
  if( (m_isWeather_first_get == true) || ((millis() - m_weather_get_last_time) > get_interval) ){
    YahooJrssGet::getYahooJweather( host, target_url );
    m_weather_get_last_time = millis();
  }
}
//****************************************
void YahooJrssGet::scrolleYahooJnews( FontParameter &news_font, ScrolleParameter &scl_set, uint8_t *sj_txt, uint8_t font_buf[][16] ){
  if( m_isNews_first_get == false ){
    if( m_isNews_get == true ){
      scl_set.font_count = 0;
      scl_set.font_sjis_len = SFR.convStrToSjis( m_news_str, sj_txt );
      scl_set.full_or_half = SFR.convSjisToFontInc( sj_txt, scl_set.font_sjis_len, &scl_set.font_count, font_buf );
      //Serial.printf( "m_news_sj_length = %d\r\n", m_news_sj_length );
      m_news_str = "";
      //Serial.printf("Free Heap Size ( After News Get ) = %d\r\n", esp_get_free_heap_size());
      m_isNews_get = false;
      scl_set.single_fnt_scl_cnt = 0;
      scl_set.full_or_half_cnt = 0;
    }

    if( LCD.scrolle8x16fontInc( news_font, scl_set, scl_set.font_sjis_len, font_buf ) ){
      scl_set.full_or_half = SFR.convSjisToFontInc( sj_txt, scl_set.font_sjis_len, &scl_set.font_count, font_buf );
    }
  }
}
//****************************************************************
void YahooJrssGet::myFontReadSd(File F, uint8_t ZorH, uint8_t num, uint8_t buf[2][16]){
  F.seek(num * (16 * ZorH));
  F.read(buf[0], 16);
  F.read(buf[1], 16);
}

}  // namespace mynamespace
