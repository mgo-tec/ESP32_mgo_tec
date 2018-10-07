/*
  display_shinonome_fnt.cpp - for Arduino core for the ESP32.
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
*/

#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/display_shinonome_fnt.h"

// In the display_shinonome_fnt.cpp file
namespace mgo_tec_esp32_bv1 {
// Definition of functions is within scope of the namespace.

//********************************************************************
void DispShinonomeFnt::initScrolle( FontParameter &font, ScrolleParameter &scl_set ){
  LCD.XscrolleFontArrayInit( font, scl_set, scl_set.disp_txt_len, font.Xsize, font.Ysize);
  LCD.scrolleFontSetUp( font, scl_set );
}
//********************************************************************
uint8_t DispShinonomeFnt::createNewSingleArray( uint16_t ary_size ){
  if( mp_isHeap_create == true ){
    log_v("Cannot create new heap memory.");
    log_v("heap size = %d", esp_get_free_heap_size());
    DispShinonomeFnt::deleteSingleArray();
  }
  //Serial.printf("Before Create New Free Heap Size = %d\r\n", esp_get_free_heap_size());

  mp_heap_array_size = ary_size;
  mp_heap_single_array = new uint8_t [ ary_size ];
  //if(mp_heap_single_array == NULL) return 0;

  for(int i = 0; i < ary_size; i++){
    mp_heap_single_array[i] = 0;
  }

  mp_isHeap_create = true;
  log_v("Shift_JIS Array Create New.");
  log_v("After Free Heap Size = %d", esp_get_free_heap_size());
  return 1;
}
//********************************************************************
void DispShinonomeFnt::deleteSingleArray(){
  if( mp_isHeap_create == false ){
    log_v("Cannot create. Delete heap memory.");
    log_v("heap size = %d", esp_get_free_heap_size());
    log_v("First, you should create heap memory!");
    return;
  }
  //Serial.printf("Before Delete Free Heap Size = %d\r\n", esp_get_free_heap_size());

  delete[] mp_heap_single_array;

  mp_isHeap_create = false;
  log_v("Shift_JIS Array Delete.");
  log_v("After Free Heap Size = %d", esp_get_free_heap_size());
}
//****************************************
void DispShinonomeFnt::scrolleText( FontParameter &font, ScrolleParameter &scl_set ){
  if( LCD.scrolle8x16fontInc( font, scl_set, scl_set.font_sjis_len, mp_font_buf ) ){
    scl_set.full_or_half = SFR.convSjisToFontInc( mp_heap_single_array, scl_set.font_sjis_len, &scl_set.font_count, mp_font_buf);
  }
}
//****************************************
void DispShinonomeFnt::newSetText( ScrolleParameter &scl_set, String str ){
  //UTF-8は2～3バイトコードのため、Shift_JISより長い。
  //とりあえず、UTF-8の長さで長めの配列確保
  //文字列長さ０の場合はスペースを挿入しないと、文字化けする。
  uint16_t len = str.length();
  if( len == 0 ) {
    log_v("str.length = %d", len);
    log_v("Add Space.");
    str = "　"; //全角スペース
  }
  DispShinonomeFnt::createNewSingleArray( str.length() );
  
  //ここで正しいShift_JISコード長を取得
  scl_set.font_sjis_len = SFR.convStrToSjis( str, mp_heap_single_array );
  log_v("scl_set.font_sjis_len = %d", scl_set.font_sjis_len);
  //※convSjisToFontInc関数の前にfont_countをゼロにすること。
  //後にすると正常に表示されないので注意。
  scl_set.font_count = 0;
  scl_set.full_or_half = SFR.convSjisToFontInc( mp_heap_single_array, scl_set.font_sjis_len, &scl_set.font_count, mp_font_buf);
  scl_set.single_fnt_scl_cnt = 0;
  scl_set.full_or_half_cnt = 0;
}
//****************************************
uint16_t DispShinonomeFnt::dispText( String str ){
  FontParameter font;
  return DispShinonomeFnt::dispText( font, str );
}
//****************************************
uint16_t DispShinonomeFnt::dispText( FontParameter &font, String str ){
  //UTF-8は2～3バイトコードのため、Shift_JISより長い。
  //ひとまず、UTF-8コードによる長めの配列確保
  uint16_t len = str.length();
  if( len == 0 ) {
    log_v("str.length = %d", len);
    log_v("Add Space.");
    //文字列長さ０の場合はスペースを挿入しないと、文字化けする。
    str = "　"; //全角スペース
  }
  uint8_t font_array[ len ][ 16 ] = {};
  
  //ここで正しいShift_JISコード長を取得
  len = SFR.convStrToFont( str, font_array );
  font.txt_width = len;
  log_v("font.txt_width = %d", len );
  
  LCD.display8x16Font( font, font.txt_width, font_array );
  return len;
}

}  // namespace mynamespace
