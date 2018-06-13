/*
  shinonome.cpp - Arduino core for the ESP32 Library.
  Beta version 1.0
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

#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h"

// In the shinonome.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.

SdShinonomeFont::SdShinonomeFont(uint8_t cs, uint32_t sd_freq)
: cs_(cs), sd_freq_(sd_freq)
{}

//*********東雲フォントライブラリ初期化3ファイル*************************************************************
bool SdShinonomeFont::init3File(const char* utf8sjis_file, const char* Shino_Half_Font_file, const char* Shino_Zen_Font_file)
{
  //Serial.begin(115200);
  gf1_ = Shino_Half_Font_file;
  gf2_ = Shino_Zen_Font_file;
  
  SD.begin(cs_, SPI, sd_freq_, "/sd");
  
  Serial.println(F("card initialized."));
  mp_utf8sjis_table_file = SD.open(utf8sjis_file, FILE_READ);
  if (!mp_utf8sjis_table_file) {
    Serial.print(utf8sjis_file);
    Serial.println(F(" File not found"));
    return false;
  }else{
    Serial.print(utf8sjis_file);
    Serial.println(F(" File read OK!"));
  }
  mp_half_shino_file = SD.open(Shino_Half_Font_file, FILE_READ);
  if (!mp_half_shino_file) {
    Serial.print(Shino_Half_Font_file);
    Serial.print(F(" File not found"));
    return false;
  }else{
    Serial.print(Shino_Half_Font_file);
    Serial.println(F(" File read OK!"));
  }
  mp_full_shino_file = SD.open(Shino_Zen_Font_file, FILE_READ);
  if (!mp_full_shino_file) {
    Serial.print(Shino_Zen_Font_file);
    Serial.print(F(" File not found"));
    return false;
  }else{
    Serial.print(Shino_Zen_Font_file);
    Serial.println(F(" File read OK!"));
  }
  
  return true;
}
//********東雲フォントライブラリ初期化　２ファイル*************************************************************
bool SdShinonomeFont::init2File(const char* Shino_Half_Font_file, const char* Shino_Zen_Font_file)
{
  //Serial.begin(115200);
  
  SD.begin(cs_, SPI, sd_freq_, "/sd");

  mp_half_shino_file = SD.open(Shino_Half_Font_file, FILE_READ);
  if (!mp_half_shino_file) {
    Serial.print(Shino_Half_Font_file);
    Serial.print(F(" File not found"));
    return false;
  }else{
    Serial.print(Shino_Half_Font_file);
    Serial.println(F(" File read OK!"));
  }
  mp_full_shino_file = SD.open(Shino_Zen_Font_file, FILE_READ);
  if (!mp_full_shino_file) {
    Serial.print(Shino_Zen_Font_file);
    Serial.print(F(" File not found"));
    return false;
  }else{
    Serial.print(Shino_Zen_Font_file);
    Serial.println(F(" File read OK!"));
  }
  
  return true;
}
//**************３ファイル　フォントファイルクローズ********************
void SdShinonomeFont::close3File(){
  delay(1);
  mp_utf8sjis_table_file.close();
  delay(1);
  mp_half_shino_file.close();
  delay(1);
  mp_full_shino_file.close();
  delay(1);
  Serial.println(F("--------------3 files closed"));
}

//*************２ファイル　フォントファイルクローズ********************
void SdShinonomeFont::close2File(){
  delay(1);
  mp_half_shino_file.close();
  delay(1);
  mp_full_shino_file.close();
  delay(1);
  Serial.println(F("--------------2 files closed"));
}
//*******************東雲フォント全変換*************************
uint16_t SdShinonomeFont::convStrToFont(String str, uint8_t font_buf[][16])
{
  uint8_t sj_txt[str.length()];
  uint16_t sj_length;
  
  SPI.setFrequency(sd_freq_); //他のデバイスの速度やモードが異なるため、毎回呼び出す。
  SPI.setDataMode(SPI_MODE0);
  
  mp_utf8sjis.convUtf8ToSjisF(mp_utf8sjis_table_file, str, sj_txt, &sj_length);
  SdShinonomeFont::convSjisToFontF2All(mp_full_shino_file, mp_half_shino_file, 0, 0, sj_txt, sj_length, font_buf);
  return sj_length;
}
//*******************東雲フォント全変換 UTF8toShis変換ファイルハンドル外部読み込み*************************
uint16_t SdShinonomeFont::convStrToFontF(File utf8sjis_table_file, String str, uint8_t font_buf[][16])
{
  uint8_t sj_txt[str.length()];
  uint16_t sj_length;
  
  SPI.setFrequency(sd_freq_); //他のデバイスの速度やモードが異なるため、毎回呼び出す。
  SPI.setDataMode(SPI_MODE0);
  
  mp_utf8sjis.convUtf8ToSjisF(utf8sjis_table_file, str, sj_txt, &sj_length);
  SdShinonomeFont::convSjisToFontF2All(mp_full_shino_file, mp_half_shino_file, 0, 0, sj_txt, sj_length, font_buf);
  return sj_length;
}
//*******************東雲フォント全変換（Shift_JISコード返りあり）*************************
uint16_t SdShinonomeFont::convStrToSjisToFont(String str, uint8_t* sj_code, uint8_t font_buf[][16])
{
  uint16_t sj_length;
  
  SPI.setFrequency(sd_freq_); //他のデバイスの速度やモードが異なるため、毎回呼び出す。
  SPI.setDataMode(SPI_MODE0);
  
  mp_utf8sjis.convUtf8ToSjisF(mp_utf8sjis_table_file, str, sj_code, &sj_length);
  SdShinonomeFont::convSjisToFontF2All(mp_full_shino_file, mp_half_shino_file, 0, 0, sj_code, sj_length, font_buf);
  return sj_length;
}

//*******************東雲フォント１文字変換*************************************************************
uint8_t SdShinonomeFont::convSjisToFontF2(File f1, File f2, uint8_t Direction, int16_t Angle, uint8_t jisH, uint8_t jisL, uint8_t buf1[16], uint8_t buf2[16])
{
  uint16_t fnt_adrs_half = 0x1346; //space
  uint32_t fnt_adrs_Zen = 0x467; //Space
  uint8_t cp;
  
  if((jisH>=0x20 && jisH<=0x7E) || (jisH>=0xA1 && jisH<=0xDF)){
    if(jisH<=0x63) fnt_adrs_half = 0x1346+(jisH-0x20)*126;
    else if(jisH<=0x7E) fnt_adrs_half = 0x34BF+(jisH-0x64)*127;
    else if(jisH>=0xA1) fnt_adrs_half = 0x4226+(jisH-0xA1)*129;

    SdShinonomeFont::convTableAdrsToHarfFont(f2, fnt_adrs_half, buf1);
    cp = 1;
    
    if((jisL>=0x20 && jisL<=0x7E) || (jisL>=0xA1 && jisL<=0xDF)){
      if(jisL<=0x63) fnt_adrs_half = 0x1346+(jisL-0x20)*126;
      else if(jisL<=0x7E) fnt_adrs_half = 0x34BF+(jisL-0x64)*127;
      else if(jisL>=0xA1) fnt_adrs_half = 0x4226+(jisL-0xA1)*129;
      
      SdShinonomeFont::convTableAdrsToHarfFont(f2, fnt_adrs_half, buf2);
      cp = 2;
    }else{
      cp = 1;
    }
  }else{
    SdShinonomeFont::convSjisToFontAdrs(jisH, jisL, &fnt_adrs_Zen);
    SdShinonomeFont::convTableAdrsToFullFont(f1, fnt_adrs_Zen, buf1, buf2);
    cp = 2;
  }
  return cp;
}
//*******************東雲フォント１文字変換***************************************************
uint8_t SdShinonomeFont::convSjisToFont(uint8_t jisH, uint8_t jisL, uint8_t buf1[16], uint8_t buf2[16]){
  uint16_t fnt_adrs_half = 0x1346; //space
  uint32_t fnt_adrs_Zen = 0x467; //Space
  uint8_t cp;
  
  SPI.setFrequency(sd_freq_); //他のデバイスの速度やモードが異なるため、毎回呼び出す。
  SPI.setDataMode(SPI_MODE0);
  
  if((jisH>=0x20 && jisH<=0x7E) || (jisH>=0xA1 && jisH<=0xDF)){
    if(jisH<=0x63) fnt_adrs_half = 0x1346+(jisH-0x20)*126;
    else if(jisH<=0x7E) fnt_adrs_half = 0x34BF+(jisH-0x64)*127;
    else if(jisH>=0xA1) fnt_adrs_half = 0x4226+(jisH-0xA1)*129;

    SdShinonomeFont::convTableAdrsToHarfFont(mp_half_shino_file, fnt_adrs_half, buf1);
    cp = 1;
    
    if((jisL>=0x20 && jisL<=0x7E) || (jisL>=0xA1 && jisL<=0xDF)){
      if(jisL<=0x63) fnt_adrs_half = 0x1346+(jisL-0x20)*126;
      else if(jisL<=0x7E) fnt_adrs_half = 0x34BF+(jisL-0x64)*127;
      else if(jisL>=0xA1) fnt_adrs_half = 0x4226+(jisL-0xA1)*129;
      
      SdShinonomeFont::convTableAdrsToHarfFont(mp_half_shino_file, fnt_adrs_half, buf2);
      cp = 2;
    }else{
      cp = 1;
    }
  }else{
    SdShinonomeFont::convSjisToFontAdrs(jisH, jisL, &fnt_adrs_Zen);
    SdShinonomeFont::convTableAdrsToFullFont(mp_full_shino_file, fnt_adrs_Zen, buf1, buf2);
    cp = 2;
  }
  return cp;
}
//*************S-jisまとめて変換*****************************************
void SdShinonomeFont::convSjisToFontF2All(File f1, File f2, uint8_t Direction, int16_t Angle, uint8_t* Sjis, uint16_t sj_length, uint8_t font_buf[][16])
{
  uint16_t fnt_adrs_half = 0x1346; //space
  uint32_t fnt_adrs_Zen = 0x467; //Space
  uint16_t i = 0;
  
  SPI.setFrequency(sd_freq_); //他のデバイスの速度やモードが異なるため、毎回呼び出す。
  SPI.setDataMode(SPI_MODE0);
  
  while(i < sj_length){
    if(Sjis[i] < 0x20) Sjis[i] = 0x20; //制御コードは全てスペース
    if((Sjis[i]>=0x20 && Sjis[i]<=0x7E) || (Sjis[i]>=0xA1 && Sjis[i]<=0xDF)){

      if(Sjis[i]<=0x63) fnt_adrs_half = 0x1346+(Sjis[i]-0x20)*126;
      else if(Sjis[i]<=0x7E) fnt_adrs_half = 0x34BF+(Sjis[i]-0x64)*127;
      else if(Sjis[i]>=0xA1) fnt_adrs_half = 0x4226+(Sjis[i]-0xA1)*129;

      SdShinonomeFont::convTableAdrsToHarfFont(f2, fnt_adrs_half, font_buf[i]);

      if(Sjis[i+1] < 0x20) Sjis[i+1] = 0x20; //制御コードは全てスペース
      if((Sjis[i+1]>=0x20 && Sjis[i+1]<=0x7E) || (Sjis[i+1]>=0xA1 && Sjis[i+1]<=0xDF)){
        if(Sjis[i+1]<=0x63) fnt_adrs_half = 0x1346+(Sjis[i+1]-0x20)*126;
        else if(Sjis[i+1]<=0x7E) fnt_adrs_half = 0x34BF+(Sjis[i+1]-0x64)*127;
        else if(Sjis[i+1]>=0xA1) fnt_adrs_half = 0x4226+(Sjis[i+1]-0xA1)*129;

        SdShinonomeFont::convTableAdrsToHarfFont(f2, fnt_adrs_half, font_buf[i+1]);
        i = i+2;
      }else{
        i++;
      }
    }else{
      SdShinonomeFont::convSjisToFontAdrs(Sjis[i], Sjis[i+1], &fnt_adrs_Zen);
      SdShinonomeFont::convTableAdrsToFullFont(f1, fnt_adrs_Zen, font_buf[i], font_buf[i+1]);
      i = i+2;
    }
    yield();
  }
}
//*******************Shift_JISコードから東雲フォントアドレス計算********************************************
void SdShinonomeFont::convSjisToFontAdrs(uint8_t jisH, uint8_t jisL, uint32_t* fnt_adrs) 
{    // S-JISコードから東雲フォントファイル上のバイト位置をポインタで返す。
  uint16_t jisCode;
  int32_t adj = 0;
  
  if( jisH != '\0'){  //'\0'ならば読み込まない。
    jisCode = ((uint16_t)jisH << 8 )|jisL;
    if(jisCode>=0x8140 && jisCode <=0x83D6){
      if     (jisCode<=0x817E) adj = 1127;  // スペース～×
      else if(jisCode<=0x81AC) adj = 963;  // ÷～〓
      else if(jisCode<=0x81BF) adj = -841;  // ∈～∩
      else if(jisCode<=0x81CE) adj = -2153;  // ∧～∃
      else if(jisCode<=0x81E8) adj = -3957;  // ∠～∬
      else if(jisCode<=0x81F7) adj = -5105;  // Å～¶
      else if(jisCode==0x81FC) adj = -5761;  // ◯
      else if(jisCode<=0x8258) adj = -19209;  // 一般記号
      else if(jisCode<=0x8279) adj = -20357;  // 数字、英語大文字
      else if(jisCode<=0x829A) adj = -21505;  // 英小文字、ひらがな
      else if(jisCode<=0x82F1) adj = -22161;  // 英小文字、ひらがな
      else if(jisCode<=0x837e) adj = -34953;  // カタカナ
      else if(jisCode<=0x8396) adj = -35117;  // カタカナ
      else if(jisCode<=0x83B6) adj = -36429;  // Α～Ω
      else if(jisCode>=0x83BF) adj = -37741;  // α～ω
      *fnt_adrs = (jisCode-0x8140)*164 + adj;
    }else if(jisCode>=0x8440 && jisCode <=0x84BE){
      if(jisCode<=0x8460) adj = 70992;  // А～Я
      else if(jisCode<=0x847E) adj = 68517;  // а～н
      else if(jisCode<=0x8491) adj = 68352;  // о～я
      else if(jisCode>=0x849F) adj = 66207;  // 罫線
      *fnt_adrs = (jisCode-0x8440)*165 + adj;
    }else if(jisCode>=0x8740 && jisCode <=0x879E){ //JIS13区は無いので"？"マークにする
      *fnt_adrs = 0x987; //"？"文字
    }else if(jisCode>=0x889f && jisCode<=0x88FC){ //JIS第１水準 亜～蔭
      *fnt_adrs = (jisCode-0x889F)*165+87162;
    }else if(jisCode>=0x8940 && jisCode<=0x9872){ //JIS第１水準 院～腕
      if(jisL <= 0x7E){
        *fnt_adrs = (jisCode-0x889F)*165+87162 -(jisH-0x88)*11055-(jisH-0x88-1)*165;
      }else if(jisL >= 0x80 ){
        *fnt_adrs = (jisCode-0x889F)*165+87162-(jisH-0x88)*11055-(jisH-0x88)*165;
      }
    }else if(jisCode>=0x989F && jisCode<=0x9FFC){ //JIS第２水準 弌～滌
      if(jisL <= 0x7E){
        *fnt_adrs = (jisCode-0x889F)*165+80067-(jisH-0x88)*11055-(jisH-0x88-1)*165;
      }else if(jisL >= 0x80 ){
        *fnt_adrs = (jisCode-0x889F)*165+80067-(jisH-0x88)*11055-(jisH-0x88)*165;
      }
    }else if(jisCode>=0xE040 && jisCode <=0xEAA4){ //JIS第２水準 漾～熙
      if(jisL <= 0x7E){
        *fnt_adrs = (jisCode-0x889F)*165-2634348-(jisH-0xC9)*11055-(jisH-0xC9)*165;
      }else if(jisL >= 0x80 ){
        *fnt_adrs = (jisCode-0x889F)*165-2634348-(jisH-0xC9)*11055-(jisH-0xC9+1)*165;
      }
    }else{
      *fnt_adrs = 0x467;  // 対応文字コードがなければ 全角スペースを返す
    }
  }else {
    *fnt_adrs = 0x467;  // 対応文字コードがなければ 全角スペースを返す
  }
}

//*****************全角フォントファイル読み込み、ファイルハンドル渡し**************************************
void SdShinonomeFont::convTableAdrsToFullFont(File shino_full_font_file, uint32_t addrs, uint8_t buf1[16], uint8_t buf2[16])
{
//  Dir dir = SD.openDir("/");//これは読み取り速度が遅くなるかもしれないのでコメントアウトしている
  uint8_t i;
  uint8_t j=0;
  uint8_t c1, c2, c3, c4;
  uint8_t c[85];
  if(shino_full_font_file){
    shino_full_font_file.seek(addrs);
    shino_full_font_file.read(c,80); //4byte+"." -->5byte*16=80byte
    for (i=0; i<16; i++){
      if(c[j]>=0x61) c1 = (c[j]-0x61)+10;
      else c1 = c[j]-0x30;
      if(c[j+1]>=0x61) c2 = (c[j+1]-0x61)+10;
      else c2 = c[j+1]-0x30;
      if(c[j+2]>=0x61) c3 = (c[j+2]-0x61)+10;
      else c3 = c[j+2]-0x30;
      if(c[j+3]>=0x61) c4 = (c[j+3]-0x61)+10;
      else c4 = c[j+3]-0x30;

      buf1[i] = (c1<<4)|c2;
      buf2[i] = (c3<<4)|c4;
      j=j+5;
    }
  }else{
    Serial.println(F(" shinonome file has not been uploaded to the flash in SD file system"));
    delay(30000);
  }
}

//*****************半角フォントファイル読み込み、ファイルハンドル渡し**************************************
void SdShinonomeFont::convTableAdrsToHarfFont(File shino_half_font_file, uint32_t addrs, uint8_t buf[16])
{
//  Dir dir = SD.openDir("/");//これは読み取り速度が遅くなるかもしれないのでコメントアウトしている
  uint8_t i;
  uint8_t j=0;
  uint8_t c1, c2;
  uint8_t c[50];
  
  if(shino_half_font_file){
    shino_half_font_file.seek(addrs);
    shino_half_font_file.read(c,48); //2byte+"." -->3byte*16=48byte
    for (i=0; i<16; i++){
      if(c[j]>=0x61) c1 = (c[j]-0x61)+10;
      else c1 = c[j]-0x30;
      if(c[j+1]>=0x61) c2 = (c[j+1]-0x61)+10;
      else c2 = c[j+1]-0x30;

      buf[i] = (c1<<4)|c2;
      j=j+3;
    }
  }else{
    Serial.println(F(" Shinonome file has not been uploaded to the flash in SD file system"));
    delay(30000);
  }
}
//*******************Shift_JIS 全角半角判断*************************************************************
uint8_t SdShinonomeFont::convSjisToFontInc(uint8_t *sj, uint16_t length, uint16_t *sj_cnt, uint8_t buf[2][16]){
  uint16_t fnt_adrs_half = 0x1346; //space
  uint32_t fnt_adrs_Zen = 0x467; //Space
  uint8_t cp;
  
  SPI.setFrequency(sd_freq_); //他のデバイスの速度やモードが異なるため、毎回呼び出す。
  SPI.setDataMode(SPI_MODE0);
  
  if((*(sj + *sj_cnt)>=0x20 && *(sj + *sj_cnt)<=0x7E) || (*(sj + *sj_cnt)>=0xA1 && *(sj + *sj_cnt)<=0xDF)){
    if(*(sj + *sj_cnt)<=0x63) fnt_adrs_half = 0x1346+(*(sj + *sj_cnt)-0x20)*126;
    else if(*(sj + *sj_cnt)<=0x7E) fnt_adrs_half = 0x34BF+(*(sj + *sj_cnt)-0x64)*127;
    else if(*(sj + *sj_cnt)>=0xA1) fnt_adrs_half = 0x4226+(*(sj + *sj_cnt)-0xA1)*129;

    SdShinonomeFont::convTableAdrsToHarfFont(mp_half_shino_file, fnt_adrs_half, buf[0]);
    cp = 1;
    
    if((*(sj + *sj_cnt+1)>=0x20 && *(sj + *sj_cnt+1)<=0x7E) || (*(sj + *sj_cnt+1)>=0xA1 && *(sj + *sj_cnt+1)<=0xDF)){
      if(*(sj + *sj_cnt+1)<=0x63) fnt_adrs_half = 0x1346+(*(sj + *sj_cnt+1)-0x20)*126;
      else if(*(sj + *sj_cnt+1)<=0x7E) fnt_adrs_half = 0x34BF+(*(sj + *sj_cnt+1)-0x64)*127;
      else if(*(sj + *sj_cnt+1)>=0xA1) fnt_adrs_half = 0x4226+(*(sj + *sj_cnt+1)-0xA1)*129;
      
      SdShinonomeFont::convTableAdrsToHarfFont(mp_half_shino_file, fnt_adrs_half, buf[1]);
      cp = 2;
    }else{
      cp = 1;
    }
  }else{
    SdShinonomeFont::convSjisToFontAdrs(*(sj + *sj_cnt), *(sj + *sj_cnt+1), &fnt_adrs_Zen);
    SdShinonomeFont::convTableAdrsToFullFont(mp_full_shino_file, fnt_adrs_Zen, buf[0], buf[1]);
    cp = 2;
  }
  
  *sj_cnt = *sj_cnt + cp;
  if(*sj_cnt >= length) *sj_cnt = 0;
  
  return cp;
}
//*******************UTF8 -> Shift_JISコード変換のみ*************************
uint16_t SdShinonomeFont::convStrToSjis(String str, uint8_t* sj_code){
  uint16_t sj_length;

  mp_utf8sjis.convUtf8ToSjisF(mp_utf8sjis_table_file, str, sj_code, &sj_length);
  return sj_length;
}

}  // namespace mynamespace
