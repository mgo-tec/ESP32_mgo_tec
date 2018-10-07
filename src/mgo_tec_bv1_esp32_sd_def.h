/*
  mgo_tec_bv1_esp32_sd_def.h
  Beta version 1.0.0
*/
#ifndef MGO_TEC_BV1_ESP32_SD_DEF_H_INCLUDED_
#define MGO_TEC_BV1_ESP32_SD_DEF_H_INCLUDED_

#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h"

  #ifdef MGO_TEC_BV1_M5STACK_SD_SKETCH
    mgo_tec_esp32_bv1::ILI9341Spi LCD;
    mgo_tec_esp32_bv1::SdShinonomeFont SFR;
  #endif

#endif
