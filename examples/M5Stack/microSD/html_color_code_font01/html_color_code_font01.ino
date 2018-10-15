#define MGO_TEC_BV1_M5STACK_SD_SKETCH
#include <mgo_tec_bv1_m5stack_sd_simple1.h>

/* Font file Reference BLOG:
 *  https://www.mgo-tec.com/font-lib-inst-spiffs-arduino-esp32
 */
const char* utf8sjis_file = "/font/Utf8Sjis.tbl"; //UTF8 Shift_JIS 変換テーブルファイル名を記載しておく
const char* shino_full_font_file = "/font/shnmk16.bdf"; //オリジナル東雲全角フォントファイル
const char* shino_half_font_file = "/font/shnm8x16.bdf"; //半角フォントファイル名を定義

void setup() {
  Serial.begin(115200);
  mM5.init( utf8sjis_file, shino_half_font_file, shino_full_font_file );
  //font[max=8], scl_set[max=8], disp_fnt[max=8]

  mM5.font[0].x0 = 100; mM5.font[0].y0 = 0;
  mM5.font[0].htmlColorCode( "#FFFFFF" );
  mM5.font[0].Xsize = 1, mM5.font[0].Ysize = 1;
  mM5.disp_fnt[0].dispText( mM5.font[0], "Hello World!" );

  mM5.font[0].x0 = 16; mM5.font[0].y0 = 16;
  mM5.font[0].htmlColorCode( "white" );
  mM5.font[0].htmlBgColorCode( "#FF0000" );
  mM5.font[0].Xsize = 2, mM5.font[0].Ysize = 2;
  mM5.disp_fnt[0].dispText( mM5.font[0], "こんにちは。世界！" );

  mM5.font[0].x0 = 48; mM5.font[0].y0 = 50;
  mM5.font[0].htmlColorCode( "#00FF00" );
  mM5.font[0].htmlBgColorCode( "black" );
  mM5.font[0].Xsize = 3, mM5.font[0].Ysize = 7;
  mM5.disp_fnt[0].dispText( mM5.font[0], "日本語漢字" );

  mM5.font[1].x0 = 32; mM5.font[1].y0 = 160;
  mM5.font[1].htmlColorCode( "blue" );
  mM5.font[1].htmlBgColorCode( "yellow" );
  mM5.font[1].Xsize = 1, mM5.font[1].Ysize = 2;
  String str = "文字表示関数が簡単になりました！";
  mM5.disp_fnt[1].dispText( mM5.font[1], str );

  mM5.font[1].x0 = 0; mM5.font[1].y0 = 192;
  mM5.font[1].htmlColorCode( "black" );
  mM5.font[1].htmlBgColorCode( "cyan" );
  mM5.font[1].Xsize = 2, mM5.font[1].Ysize = 2;
  str = " HTML Color Code入力";
  mM5.disp_fnt[1].dispText( mM5.font[1], str );

  mM5.font[1].x0 = 0; mM5.font[1].y0 = 224;
  mM5.font[1].htmlColorCode( "magenta" );
  mM5.font[1].htmlBgColorCode( "black" );
  mM5.font[1].Xsize = 1, mM5.font[1].Ysize = 1;
  str = "東雲フォント、半角ｶﾀｶﾅもOK.ｱｲｳｴｵｧｨｩｪｫ";
  mM5.disp_fnt[1].dispText( mM5.font[1], str );
}

void loop() {
}
