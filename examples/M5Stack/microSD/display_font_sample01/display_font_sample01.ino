#define MGO_TEC_BV1_M5STACK_SD_SKETCH
#include <mgo_tec_bv1_m5stack_sd_simple1.h>

/* Font file Reference BLOG:
 *  https://www.mgo-tec.com/font-lib-inst-spiffs-arduino-esp32
 */
const char* utf8sjis_file = "/font/Utf8Sjis.tbl"; //UTF8 Shift_JIS 変換テーブルファイル名を記載しておく
const char* shino_full_font_file = "/font/shnmk16.bdf"; //オリジナル東雲全角フォントファイル
const char* shino_half_font_file = "/font/shnm8x16.bdf"; //半角フォントファイル名を定義

void setup() {
  mM5.init( utf8sjis_file, shino_half_font_file, shino_full_font_file );

  //font[max=8], scl_set[max=8], disp_fnt[max=8]
  mM5.font[0].x0 = 50; mM5.font[0].y0 = 30;
  mM5.disp_fnt[0].dispText( mM5.font[0], "これはテストです" );

  String str1 = "Hello World M5Stack ! ハローワールド　日本語漢字　";
  mM5.font[1].y0 = 150;
  mM5.scl_set[1].interval = 10; //(millisecond)
  mM5.disp_fnt[1].initScrolle( mM5.font[1], mM5.scl_set[1] );
  mM5.disp_fnt[1].newSetText( mM5.scl_set[1], str1 );
}

void loop() {
  mM5.disp_fnt[1].scrolleText( mM5.font[1], mM5.scl_set[1] );
}
