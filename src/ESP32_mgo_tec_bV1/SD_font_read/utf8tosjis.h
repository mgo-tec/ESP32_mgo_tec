/*
  utf8tosjis.h - Arduino core for the ESP32 Library( use SD ).
  Beta version 1.0
  
  This is a library for converting from UTF-8 code string to Shift_JIS code string.
  In advance, you need to upload a conversion table file Utf8Sjis.tbl using micro SD card & ESP-WROOM-32 ( ESP32 ).
  
The MIT License (MIT)

Copyright (c) 2018 Mgo-tec
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

#ifndef MGO_TEC_ESP32_SD_UTF8TOSJIS_H_INCLUDED_
#define MGO_TEC_ESP32_SD_UTF8TOSJIS_H_INCLUDED_
#include <Arduino.h>
#include <SD.h>

// In the utf8tosjis.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

class SdUtf8toSjis
{
public:

  void init( const char* utf8sjis_file );
  void fileClose();
  uint16_t convUtf8ToSjis( String utf8_str, uint8_t* sjis_byte );
  void convUtf8ToSjisF( File table_file, String utf8_str, uint8_t* sjis_byte, uint16_t* sj_length );
  void codeConvUtf8ToSjis( uint8_t utf8_1, uint8_t utf8_2, uint8_t utf8_3, uint32_t* spiffs_addrs );
  void readTableUtf8SjisF( File table_file, uint32_t addrs, uint8_t* buf );

private:
  File mp_utf8sjis_table_file;
};

}// namespace mgo_tec_esp32_bv1

#endif
