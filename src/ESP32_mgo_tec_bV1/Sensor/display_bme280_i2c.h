/*
  display_bme280_i2c.h - for Arduino core for the ESP32.
  Beta version 1.0.0
  
The MIT License (MIT)

Copyright (c) 2018 Mgo-tec. All rights reserved.
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

Use BOSCH BME280 Driver.
URL: https://github.com/BoschSensortec/BME280_driver
Copyright (C) 2016 - 2017 Bosch Sensortec GmbH

*/

#ifndef _MGO_TEC_ESP32_DISPLAY_BME280_I2C_H_INCLUDED
#define _MGO_TEC_ESP32_DISPLAY_BME280_I2C_H_INCLUDED

#include <Arduino.h>
#include <Wire.h>
#include "TimeLib.h" //Use Arduino time library ver1.5-
#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"
#include "ESP32_mgo_tec_bV1/SD_font_read/shinonome.h"

extern "C" {
#include "bme280.h" //Bosch Driver ( The 3-Clause BSD License )
}

extern mgo_tec_esp32_bv1::ILI9341Spi LCD;
extern mgo_tec_esp32_bv1::SdShinonomeFont SFR;

// In the button_switch.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

extern "C" {
  //Boschドライバー側で、typedef の関数ポインタがあり、C++コンパイラエラーが出る。
  //その為、この２つの関数のみクラスから外し、externで囲う
  int8_t userI2cRead( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );
  int8_t userI2cWrite( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );
}

class DisplayBme280I2c
{
private:
  //using bme280_com_fptr_t = int8_t(*)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
  struct bme280_dev dev;
  struct bme280_data comp_data;
  uint32_t mp_get_data_last_time = 0;
  uint32_t mp_disp_min_max_last_time = 0;
  String mp_prev_pres_str, mp_prev_temp_str, mp_prev_hum_str;
  boolean isTime_plot_heap_create = false;

public:
  enum DispState { NoWrite = -1, WriteDataError = 0, WriteGoodData = 1 }
    DispStatus = NoWrite;

  enum TimeMsrMod { Period3sec = 0, Period6sec, Period12sec, Period36sec, Period1m12s, Period4m48s }
    TimeMeasureMode = Period3sec;
  
  float m_fTemperature = -500.0;
  float m_fPressure = -500.0;
  float m_fHumidity = -500.0;
  int16_t m_iTemperature = -500;
  int16_t m_iPressure = -500;
  int16_t m_iHumidity = -500;
  String m_pres_str, m_temp_str, m_hum_str;

  uint32_t m_meas_period = 3000; //3000ms
  int8_t m_time_measure_mode = 0;
  boolean m_graph_disp_ok = false;
  boolean m_value_disp_ok = false;
  boolean m_isNewData = false;
  boolean m_isDisp_min_max = false;
  boolean m_isShowing_min_max = false;

  uint32_t *m_array_time_plot;

private:
  boolean createNewArray( GraphParameter &graph );
  void deleteArray( GraphParameter &graph );
  boolean createNewTimePlotArray();
  void deleteTimePlotArray();
  void userDelayMs( uint32_t period );

public:
  void initBme280Force( int sda, int scl, uint32_t freq );
  void getData();
  DispState getDataFloat( float *fT, float *fP, float *fH );
  void serialPrintSensorData();
  void initGraphAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, FontParameter &graph_font );
  void reDrawGraphAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, FontParameter &graph_font );
  void drawGraphLineAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph );
  void drawGraphNowAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph );
  void initDispValue( FontParameter &pres_font, FontParameter &temp_font, FontParameter &hum_font, int16_t x0, int16_t y0, char unit_c[][5] );
  void displayValue( FontParameter &pres_font, FontParameter &temp_font, FontParameter &hum_font );
  void displayMinMaxValue( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, FontParameter &pres_font, FontParameter &temp_font, FontParameter &hum_font );

};

}// namespace mgo_tec_esp32_bv1

#endif
