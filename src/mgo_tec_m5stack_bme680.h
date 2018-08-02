/*
  mgo_tec_m5stack_bme680.h
  This is an include-only file for linking other files.

  beta ver 1.0.0

  Use M5Stack ( ESP32, LCD ILI9341 spi, SD spi ) and BOSCH BME680.

  Use BOSCH BME680 driver ( bme680.h )
  The 3-Clause BSD License
  Copyright (C) 2016 - 2017 Bosch Sensortec GmbH
  URL: https://github.com/BoschSensortec/BME680_driver
*/

#ifndef _MGO_TEC_M5STACK_BME680_H_INCLUDED
#define _MGO_TEC_M5STACK_BME680_H_INCLUDED

#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/utf8tosjis.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h"
#include "ESP32_mgo_tec_bV1/Web/webget.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/message_window.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/watch.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/yahooj_rss.h"
#include "ESP32_mgo_tec_bV1/Switch/button_switch.h"
#include "ESP32_mgo_tec_bV1/Sensor/display_bme680_i2c.h"

#endif
