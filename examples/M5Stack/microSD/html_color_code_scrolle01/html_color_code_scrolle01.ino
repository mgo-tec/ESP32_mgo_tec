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

  String str1 = "Hello! World Scrolle. こんにちは！ スクロールしてまっせ～！　";
  mM5.font[0].y0 = 16;
  mM5.font[0].htmlColorCode( "white" );
  mM5.font[0].htmlBgColorCode( "red" );
  mM5.font[0].Xsize = 1, mM5.font[0].Ysize = 2;
  mM5.scl_set[0].interval = 3; //(millisecond)
  mM5.disp_fnt[0].initScrolle( mM5.font[0], mM5.scl_set[0] );
  mM5.disp_fnt[0].newSetText( mM5.scl_set[0], str1 );
  String str2 = "HTMLカラーコード入力関数を追加です。";
  mM5.font[1].y0 = 100;
  mM5.font[1].htmlColorCode( "#00ff00" );
  mM5.font[1].htmlBgColorCode( "#552222" );
  mM5.font[1].Xsize = 3, mM5.font[1].Ysize = 5;
  mM5.scl_set[1].interval = 8; //(millisecond)
  mM5.disp_fnt[1].initScrolle( mM5.font[1], mM5.scl_set[1] );
  mM5.disp_fnt[1].newSetText( mM5.scl_set[1], str2 );
}

void loop() {
  mM5.disp_fnt[0].scrolleText( mM5.font[0], mM5.scl_set[0] );
  mM5.disp_fnt[1].scrolleText( mM5.font[1], mM5.scl_set[1] );
}
