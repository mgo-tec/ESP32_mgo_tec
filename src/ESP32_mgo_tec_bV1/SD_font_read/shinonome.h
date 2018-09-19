/*
  shinonome.h - Arduino core for the ESP32 Library.
  Beta version 1.0.1
  This is micro SD card library for reading Shinonome font.  
  
The MIT License (MIT)

Copyright (c) 2017 Mgo-tec
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

Licence of Shinonome Font is Public Domain.
Maintenance development of Font is /efont/.
*/

#ifndef _MGO_TEC_ESP32_SD_SHINONOMEFNT_H_INCLUDED
#define _MGO_TEC_ESP32_SD_SHINONOMEFNT_H_INCLUDED

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "ESP32_mgo_tec_bV1/SD_font_read/utf8tosjis.h"

// In the shinonome.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.


class SdShinonomeFont
{
public:
  SdShinonomeFont(uint8_t cs, uint32_t sd_freq);

  bool init3File(const char* utf8sjis_file, const char* Shino_Half_Font_file, const char* Shino_Zen_Font_file);
  bool init2File(const char* Shino_Half_Font_file, const char* Shino_Zen_Font_file);
  void close3File();
  void close2File();
  uint16_t convStrToFont(String str, uint8_t font_buf[][16]);
  uint16_t convStrToFontF(File utf8sjis_table_file, String str, uint8_t font_buf[][16]);
  uint16_t convStrToSjisToFont(String str, uint8_t* sj_code, uint8_t font_buf[][16]);
  uint8_t convSjisToFontF2(File f1, File f2, uint8_t Direction, int16_t Angle, uint8_t jisH, uint8_t jisL, uint8_t buf1[16], uint8_t buf2[16]);
  uint8_t convSjisToFont(uint8_t jisH, uint8_t jisL, uint8_t buf1[16], uint8_t buf2[16]);
  void convSjisToFontF2All(File f1, File f2, uint8_t Direction, int16_t Angle, uint8_t* Sjis, uint16_t sj_length, uint8_t font_buf[][16]);
  void convSjisToFontAdrs(uint8_t jisH, uint8_t jisL, uint32_t* fnt_adrs);
  void convTableAdrsToFullFont(File shino_full_font_file, uint32_t addrs, uint8_t buf1[16], uint8_t buf2[16]);
  void convTableAdrsToHarfFont(File shino_half_font_file, uint32_t addrs, uint8_t buf[16]);
  uint8_t convSjisToFontInc(uint8_t *sj, uint16_t length, uint16_t *sj_cnt, uint8_t buf[2][16]);

  uint16_t convStrToSjis(String str, uint8_t* sj_code);

private:
  uint8_t cs_;
  uint32_t sd_freq_;
  File mp_full_shino_file;
  File mp_half_shino_file;
  File mp_utf8sjis_table_file;
  SdUtf8toSjis mp_utf8sjis;
  
  const char *gf1_;
  const char *gf2_;

};

}// namespace mgo_tec_esp32_bv1

#endif
