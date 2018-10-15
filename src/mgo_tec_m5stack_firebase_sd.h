/*
  mgo_tec_m5stack_firebase_sd.h
  This is an include-only file for linking other files.

  beta ver 1.0.0

  Use ESP32, LCD ILI9341 spi, SD spi
*/

#ifndef MGO_TEC_M5STACK_FIREBASE_SD_H_INCLUDED_
#define MGO_TEC_M5STACK_FIREBASE_SD_H_INCLUDED_

#include "mgo_tec_m5stack_firebase_sd_init1.h"

  #ifdef MGO_TEC_BV1_M5STACK_SD_SKETCH
    mgo_tec_esp32_bv1::MgotecM5stackFirebaseSdInit1 mM5;
  #endif

#endif
