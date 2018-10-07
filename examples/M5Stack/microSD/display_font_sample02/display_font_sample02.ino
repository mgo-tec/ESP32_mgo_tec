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
  mM5.font[0].y0 = 0;
  mM5.disp_fnt[0].dispText( mM5.font[0], "これはテストです" );

  mM5.font[1].y0 = 32;
  //red:(0 - 31), green:(0 - 63), blue:(0 - 31)
  mM5.font[1].red = 31, mM5.font[1].green = 0, mM5.font[1].blue = 0;
  mM5.font[1].Xsize = 2, mM5.font[1].Ysize = 2;
  mM5.disp_fnt[1].dispText( mM5.font[1], "日本語漢字" );

  mM5.font[2].x0 = 50; mM5.font[2].y0 = 64;
  mM5.font[2].red = 0, mM5.font[2].green = 63, mM5.font[2].blue = 0;
  mM5.disp_fnt[1].dispText( mM5.font[2], "ABCDEFGabcdefg" );

  mM5.font[3].x0 = 100; mM5.font[3].y0 = 100;
  mM5.font[3].red = 0, mM5.font[3].green = 0, mM5.font[3].blue = 31;
  mM5.disp_fnt[3].dispText( mM5.font[3], "今日はいい天気" );

  String str1 = "Hello World M5Stack ! ハローワールド　日本語漢字　";
  mM5.font[4].y0 = 150;
  mM5.font[4].red = 31, mM5.font[4].green = 63, mM5.font[4].blue = 0;
  mM5.font[4].Xsize = 2, mM5.font[4].Ysize = 3;
  mM5.scl_set[4].interval = 10; //(millisecond)
  mM5.disp_fnt[4].initScrolle( mM5.font[4], mM5.scl_set[4] );
  mM5.disp_fnt[4].newSetText( mM5.scl_set[4], str1 );

  String str2 = "２行目の電光掲示板スクロールです。";
  mM5.font[5].y0 = 200;
  mM5.font[5].red = 31, mM5.font[5].green = 0, mM5.font[5].blue = 31;
  mM5.font[5].Xsize = 3, mM5.font[5].Ysize = 1;
  mM5.scl_set[5].interval = 5; //(millisecond)
  mM5.disp_fnt[5].initScrolle( mM5.font[5], mM5.scl_set[5] );
  mM5.disp_fnt[5].newSetText( mM5.scl_set[5], str2 );
}

void loop() {
  mM5.disp_fnt[4].scrolleText( mM5.font[4], mM5.scl_set[4] );
  mM5.disp_fnt[5].scrolleText( mM5.font[5], mM5.scl_set[5] );
}
