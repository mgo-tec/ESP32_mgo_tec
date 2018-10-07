/*
  display_bme680_i2c.cpp - for Arduino core for the ESP32.
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

Use BOSCH BME680 Driver.
Copyright (C) 2016 - 2017 Bosch Sensortec GmbH
The 3-Clause BSD License
URL: https://github.com/BoschSensortec/BME680_driver

Use Arduino Time Library ( TimeLib.h )
time.c - low level time and date functions
Copyright (c) Michael Margolis 2009-2014
LGPL ver2.1
https://github.com/PaulStoffregen/Time
*/

#include "ESP32_mgo_tec_bV1/Sensor/display_bme680_i2c.h"

extern "C" {
  #include "bme680.h" //Bosch Driver ( The 3-Clause BSD License )
}

// In the display_bme680_i2c.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.

//using bme680_com_fptr_t = int8_t(*)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
static struct bme680_dev gas_sensor;
static struct bme680_field_data data;

extern "C" {
  //Boschドライバー側で、typedef の関数ポインタがあり、C++コンパイラエラーが出る。
  //その為、この２つの関数のみクラスから外し、externで囲う
  static int8_t bme680UserI2cRead( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );
  static int8_t bme680UserI2cWrite( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );
}

//********************************************************************
boolean DisplayBme680I2c::createNewArray( GraphParameter &graph ){
  if( graph.isHeap_create == true ){
    Serial.printf("Cannot create new heap memory(BME680 Graph).\r\n heap size = %d\r\n", esp_get_free_heap_size());
    DisplayBme680I2c::deleteArray( graph );
  }

  graph.array_fGraph = new float[ LCD.m_max_disp_width ];
  if( graph.array_fGraph == NULL ) return false;

  graph.isHeap_create = true;
  Serial.printf("After Create New Free Heap Size(BME680 Graph)= %d\r\n", esp_get_free_heap_size());
  return true;
}
//********************************************************************
void DisplayBme680I2c::deleteArray( GraphParameter &graph ){
  if( graph.isHeap_create == false ){
    Serial.printf("Cannot create delete heap memory(BME680 Graph).\r\n heap size = %d\r\n", esp_get_free_heap_size());
    Serial.println(F("First, you should create heap memory(BME680 Graph)!"));
    return;
  }
  //Serial.printf("Before Delete Free Heap Size = %d\r\n", esp_get_free_heap_size());

  delete[] graph.array_fGraph;

  graph.isHeap_create = false;
  Serial.printf("After Delete Free Heap Size(BME680 Graph) = %d\r\n", esp_get_free_heap_size());
}
//********************************************************************
boolean DisplayBme680I2c::createNewTimePlotArray(){
  if( isTime_plot_heap_create == true ){
    Serial.printf("Cannot create new TimePlot heap memory(BME680 Graph).\r\n heap size = %d\r\n", esp_get_free_heap_size());
    DisplayBme680I2c::deleteTimePlotArray();
  }

  m_array_time_plot = new uint32_t[ LCD.m_max_disp_width ];
  if( m_array_time_plot == NULL ) return false;

  isTime_plot_heap_create = true;
  Serial.printf("After Create New Free Heap Size(BME680 Graph)= %d\r\n", esp_get_free_heap_size());
  return true;
}
//********************************************************************
void DisplayBme680I2c::deleteTimePlotArray(){
  if( isTime_plot_heap_create == false ){
    Serial.printf("Cannot create delete TimePlot heap memory(BME680 Graph).\r\n heap size = %d\r\n", esp_get_free_heap_size());
    Serial.println(F("First, you should create TimePlot heap memory(BME680 Graph)!"));
    return;
  }
  //Serial.printf("Before Delete Free Heap Size = %d\r\n", esp_get_free_heap_size());

  delete[] m_array_time_plot;

  isTime_plot_heap_create = false;
  Serial.printf("After Delete Free Heap Size(BME680 Graph) = %d\r\n", esp_get_free_heap_size());
}
//*********************************************
void DisplayBme680I2c::initBme680Force( int sda, int scl, uint32_t freq, int8_t sensor_ambient_temp ){
  Wire.begin( sda, scl, freq );
  gas_sensor.dev_id = BME680_I2C_ADDR_PRIMARY; //=0x76
  gas_sensor.intf = BME680_I2C_INTF; //I2C interface
  gas_sensor.read = bme680UserI2cRead;
  gas_sensor.write = bme680UserI2cWrite;
  gas_sensor.delay_ms = delay;
  /* amb_temp can be set to 25 prior to configuring the gas sensor 
   * or by performing a few temperature readings without operating the gas sensor.
   */
  gas_sensor.amb_temp = sensor_ambient_temp; //センサ周囲温度。ガスセンサを無効にして温度だけ測ってからその値を代入してもよい

  int8_t rslt = bme680_init(&gas_sensor);
  Serial.printf( "bme680_init rslt=%d\r\n", rslt );

  gas_sensor.tph_sett.os_hum = BME680_OS_2X;
  gas_sensor.tph_sett.os_pres = BME680_OS_4X;
  gas_sensor.tph_sett.os_temp = BME680_OS_8X;
  gas_sensor.tph_sett.filter = BME680_FILTER_SIZE_3;

  /* Set the remaining gas sensor settings and link the heating profile */
  gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
  /* Create a ramp heat waveform in 3 steps */
  gas_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
  gas_sensor.gas_sett.heatr_dur = 150; /* milliseconds */

  /* Select the power mode */
  /* Must be set before writing the sensor configuration */
  gas_sensor.power_mode = BME680_FORCED_MODE; 
  
  /* Set the required sensor settings needed */
  uint8_t set_required_settings;
  set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;
  
  /* Set the desired sensor configuration */
  rslt = bme680_set_sensor_settings(set_required_settings,&gas_sensor);
  Serial.printf( "sensor settings rslt=%d\r\n",rslt );
  
  /* Set the power mode */
  rslt = bme680_set_sensor_mode(&gas_sensor);
  Serial.printf( "sensor force mode rslt=%d\r\n", rslt );
  
  /* Get the total measurement duration so as to sleep or wait till the
   * measurement is complete */
  //bme680_get_profile_dur(&m_auto_meas_period, &gas_sensor); //ガスセンサの最低限測定可能時間取得
  bme680_set_profile_dur(m_meas_period, &gas_sensor); //meas_periodからガスセンサの最低限測定可能時間を差し引いた値をガスセンサヒーター作動時間間隔として設定する
  mp_get_data_last_time = 0;
}
//************************************************
void DisplayBme680I2c::getData(){
  //ガスセンサの測定時間間隔は一定にし、他のタスクで中断しないようにする。時間間隔を空け過ぎないようにする。
  if( millis() - mp_get_data_last_time > m_meas_period ){
    int8_t rslt = bme680_get_sensor_data(&data, &gas_sensor);
    Serial.printf( "bme680_get_sensor_data rslt=%d\r\n", rslt );
    if(rslt == BME680_OK){
      DispStatus = DisplayBme680I2c::getDataInteger( &m_fTemperature, &m_fPressure, &m_fHumidity, &m_fGas );
    }else{
      DispStatus = WriteDataError;
      Serial.println("-------- get_sensor rslt ERROR ------");
    }

    DisplayBme680I2c::serialPrintSensorData();

    if (gas_sensor.power_mode == BME680_FORCED_MODE) {
      rslt = bme680_set_sensor_mode(&gas_sensor);
      Serial.printf( "sensor force mode rslt=%d\r\n", rslt );
    }
    
    mp_get_data_last_time = millis();
    m_graph_disp_ok = true;
    m_value_disp_ok = true;
    m_gas_value_disp_ok = true;
    m_isNewData = true;
  }
}
//************************************************
DisplayBme680I2c::DispState DisplayBme680I2c::getDataInteger( float *fT, float *fP, float *fH, float *gas_data ){
  //BME680_FLOAT_ENABLE only
  *fT = (float)data.temperature / 100.0f;
  *fP = (float)data.pressure / 100.0f;
  *fH = (float)data.humidity / 1000.0f;
  
  /* Avoid using measurements from an unstable heating setup */
  if(data.status & BME680_GASM_VALID_MSK){
    *gas_data = (float)data.gas_resistance;
  }

  DispState ret_bme_data_state = WriteGoodData;

  if( *fT < -50.0 || *fT > 200.0 ){
    *fT = -500.0; // -500 is error
    ret_bme_data_state = WriteDataError;
  }
  if( *fP < 500.0 || *fP > 1300.0 ){
    *fP = -500; // -500 is error
    ret_bme_data_state = WriteDataError;
  }
  if( *fH < 0.0 || *fH > 100.0 ){
    *fH = -500; // -500 is error
    ret_bme_data_state = WriteDataError;
  }

  return ret_bme_data_state;
}
//************************************************
void DisplayBme680I2c::serialPrintSensorData(){
  //BME680_FLOAT_ENABLE only
  //m_iTemperature = (int16_t)round( m_fTemperature );
  m_iPressure = (int16_t)round( m_fPressure );
  m_iHumidity = (int16_t)round( m_fHumidity );
  m_iGas = (uint32_t)m_fGas;

  char temp_c[10], hum_c[10], pres_c[10], gas_c[20];

  switch( DispStatus ){
    case WriteDataError:
      m_pres_str = "----";
      m_temp_str = "-----";
      m_hum_str = "---";
      m_gas_str = "----";
      break;
    case WriteGoodData:
      sprintf( pres_c, "%4d", m_iPressure ); //整数値
      m_pres_str = String( pres_c );
      sprintf( temp_c, "%5.1lf", m_fTemperature ); //小数点含め全5桁、少数１桁
      m_temp_str = String( temp_c );
      sprintf( hum_c, "%3d", m_iHumidity ); //整数値
      m_hum_str = String( hum_c );
      sprintf( gas_c, "%8d", m_iGas ); //整数値
      m_gas_str = String( gas_c );
      break;
    default:
      break;
  }

  Serial.println("-----------------------");
  Serial.printf("float %7.2lf hPa, %6.2lf degreeC, %6.2lf %%\r\n", m_fPressure, m_fTemperature, m_fHumidity );
  Serial.printf( "%s hPa, %s ℃, %s ％\r\n", m_pres_str.c_str(), m_temp_str.c_str(), m_hum_str.c_str() );
  Serial.printf("Gas: %s ohms\r\n", m_gas_str.c_str() );
  Serial.println("-----------------------");
}
//**********************************************
void DisplayBme680I2c::initGraphAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, GraphParameter &gas_graph, FontParameter &graph_font ){
  DisplayBme680I2c::createNewArray( pres_graph );
  DisplayBme680I2c::createNewArray( temp_graph );
  DisplayBme680I2c::createNewArray( hum_graph );
  DisplayBme680I2c::createNewArray( gas_graph );
  DisplayBme680I2c::createNewTimePlotArray();

  DisplayBme680I2c::reDrawGraphFrame( pres_graph, temp_graph, hum_graph, gas_graph, graph_font );

  int i;
  for( i = 0; i < 320; i++ ){
    pres_graph.array_fGraph[ i ] = -500;
    temp_graph.array_fGraph[ i ] = -500;
    hum_graph.array_fGraph[ i ] = -500;
    gas_graph.array_fGraph[ i ] = 0;
    m_array_time_plot[ i ] = 0;
  }

  mp_get_data_last_time = 0;
}
//**********************************************
void DisplayBme680I2c::reDrawGraphFrame( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, GraphParameter &gas_graph, FontParameter &graph_font ){
  switch( m_time_measure_mode ){
    case 0:
      m_graph_plot_period = 3000; //3sec
      break;
    case 1:
      m_graph_plot_period = 6000; //6sec
      break;
    case 2:
      m_graph_plot_period = 12000; //12sec
      break;
    case 3:
      m_graph_plot_period = 36000; //36sec
      break;
    case 4:
      m_graph_plot_period = 72000; //1,12min
      break;
    case 5:
      m_graph_plot_period = 288000; //4.8min
      break;
    default:
      break;
  }

  pres_graph.frame_x1 = pres_graph.frame_x0 + pres_graph.x_total_points + 2;
  if( pres_graph.frame_x1 > LCD.m_max_pix_x1 ){
    pres_graph.x_total_points = LCD.m_max_pix_x1 - pres_graph.frame_x0 - 2;
    pres_graph.frame_x1 = pres_graph.frame_x0 + pres_graph.x_total_points + 2;
  }
  pres_graph.frame_y1 = pres_graph.frame_y0 + pres_graph.y_total_points + 2;
  if( pres_graph.frame_y1 > LCD.m_max_pix_y1 ){
    pres_graph.y_total_points = LCD.m_max_pix_y1 - pres_graph.frame_y0 - 2;
    pres_graph.frame_y1 = pres_graph.frame_y0 + pres_graph.y_total_points + 2;
  }

  pres_graph.x_data_min = pres_graph.frame_x0 + 1;
  pres_graph.y_data_min = pres_graph.frame_y0 + 1;
  pres_graph.x_data_max = pres_graph.frame_x1 - 1;
  pres_graph.y_data_max = pres_graph.frame_y1 - 1;

  uint16_t Y11 = pres_graph.frame_y1 + 22;
  if( Y11 > LCD.m_max_pix_y1 ) Y11 = LCD.m_max_pix_y1;

  LCD.displayClear( pres_graph.frame_x0, pres_graph.frame_y0, pres_graph.frame_x1, Y11 );
  LCD.drawRectangleLine( pres_graph.frame_x0, pres_graph.frame_y0, pres_graph.frame_x1, pres_graph.frame_y1, 31, 63, 31 );

  uint8_t graph_txt_font[40][16];

  //red(0-31), green(0-63), blue(0-31)
  graph_font.x0 = pres_graph.frame_x0;
  graph_font.y0 = pres_graph.frame_y1 + 6;
  graph_font.Xsize = 1, graph_font.Ysize = 1;
  uint16_t graph_txt_length = SFR.convStrToFont( "0", graph_txt_font );
  graph_font.x0 = pres_graph.frame_x1 - 8;
  LCD.display8x16Font( graph_font, graph_txt_length, graph_txt_font );

  //int16_t x_axis_scale = 5 * round( 60.0 / ((float)m_graph_plot_period / 1000.0) ); //5分間隔
  char mode_time_index_str[6][3][6] = {{ "-5 m", "-10 m", "-15 m" },
                                      { "-10 m", "-20 m", "-30 m" },
                                      { "-20 m", "-40 m", "-60 m" },
                                      { "-1 h", "-2 h", "-3 h" },
                                      { "-3 h", "-6 h", "-9 h" },
                                      { "-8 h", "-16 h", "-24 h" }};
  int16_t x_axis_scale = 100;
  int16_t scale_x0;

  int i;
  for( i = 0; i < 4; i++ ){
    scale_x0 = pres_graph.x_data_max - (x_axis_scale * i);
    LCD.drawLine( scale_x0, pres_graph.frame_y1, scale_x0, pres_graph.frame_y1 + 5, 31, 63, 31 );

    if( i >= 1 ){
      String graph_str = String( mode_time_index_str[ m_time_measure_mode ][ i - 1 ] );
      graph_txt_length = SFR.convStrToFont( graph_str, graph_txt_font );
      graph_font.x0 = scale_x0 - 15;
      LCD.display8x16Font( graph_font, graph_txt_length, graph_txt_font );
    }
  }
}
//**********************************************
void DisplayBme680I2c::reDrawGraphAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, GraphParameter &gas_graph, FontParameter &graph_font ){
  DisplayBme680I2c::reDrawGraphFrame( pres_graph, temp_graph, hum_graph, gas_graph, graph_font );
  DisplayBme680I2c::drawGraphLineAll( pres_graph, temp_graph, hum_graph, gas_graph );
}
//**********************************************
void DisplayBme680I2c::drawGraphLineAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, GraphParameter &gas_graph ){
  float pres_unit = 1.0 / ( (pres_graph.max_value - pres_graph.min_value)/100.0 );
  float temp_unit = 1.0 / ( (temp_graph.max_value - temp_graph.min_value)/100.0 );
  float hum_unit = 1.0 / ( (hum_graph.max_value - hum_graph.min_value)/100.0 );
  float gas_unit = 1.0 / ( (gas_graph.max_value - gas_graph.min_value)/100.0 );
  int16_t x_max = pres_graph.x_data_max;
  int16_t x_min = pres_graph.x_data_min;
  int16_t y_max = pres_graph.y_data_max;
  int16_t y_min = pres_graph.y_data_min;
  int16_t line_x0 = 0, line_x1 = 0;
  int16_t line_y0 = y_min, line_y1 = y_min;
  uint32_t min_time_plot = m_array_time_plot[ x_max ] - ( m_graph_plot_period / 1000 ) * pres_graph.x_total_points;
  uint32_t max_time_plot = m_array_time_plot[ x_max ];
  uint16_t meas_period_sec = m_graph_plot_period / 1000;
  boolean y0_ok = false, y1_ok = false;
  int i;

  LCD.displayClear( x_min, y_min, x_max, y_max );

  for( i = x_max; i >= x_min; i-- ){
    if( m_array_time_plot[ i - 1 ] < min_time_plot ) break;
    line_x0 = x_max - round( (float)( max_time_plot - m_array_time_plot[ i - 1 ] ) / (float)meas_period_sec );
    line_x1 = x_max - round( (float)( max_time_plot - m_array_time_plot[ i ] ) / (float)meas_period_sec );

    if( (pres_graph.array_fGraph[ i - 1 ] != -500) && (pres_graph.array_fGraph[ i ] != -500) ){
      line_y0 = y_max - (int)round( (pres_graph.array_fGraph[ i - 1 ] - pres_graph.min_value) * pres_unit );
      line_y1 = y_max - (int)round( (pres_graph.array_fGraph[ i ] - pres_graph.min_value) * pres_unit );
      if( line_y0 >= y_min && line_y0 <= y_max ) y0_ok = true;
      if( line_y1 >= y_min && line_y1 <= y_max ) y1_ok = true;

      if( y0_ok == true || y1_ok == true ){
        if( line_y0 < y_min ){
          line_y0 = y_min;
        }else if( line_y0 > y_max ){
          line_y0 = y_max;
        }
        if( line_y1 < y_min ){
          line_y1 = y_min;
        }else if( line_y1 > y_max ){
          line_y1 = y_max;
        }
        LCD.drawLine( line_x0, line_y0, line_x1, line_y1, pres_graph.red, pres_graph.green, pres_graph.blue );
        y0_ok = false, y1_ok = false;
      }
    }

    if( (temp_graph.array_fGraph[ i - 1 ] != -500) && (temp_graph.array_fGraph[ i ] != -500) ){
      line_y0 = y_max - (int)round( (temp_graph.array_fGraph[ i - 1 ] - temp_graph.min_value) * temp_unit );
      line_y1 = y_max - (int)round( (temp_graph.array_fGraph[ i ] - temp_graph.min_value) * temp_unit );
      if( line_y0 >= y_min && line_y0 <= y_max ) y0_ok = true;
      if( line_y1 >= y_min && line_y1 <= y_max ) y1_ok = true;

      if( y0_ok == true || y1_ok == true ){
        if( line_y0 < y_min ){
          line_y0 = y_min;
        }else if( line_y0 > y_max ){
          line_y0 = y_max;
        }
        if( line_y1 < y_min ){
          line_y1 = y_min;
        }else if( line_y1 > y_max ){
          line_y1 = y_max;
        }
        LCD.drawLine( line_x0, line_y0, line_x1, line_y1, temp_graph.red, temp_graph.green, temp_graph.blue );
        y0_ok = false, y1_ok = false;
      }
    }

    if( (hum_graph.array_fGraph[ i - 1 ] != -500) && (hum_graph.array_fGraph[ i ] != -500) ){
      line_y0 = y_max - (int)round( (hum_graph.array_fGraph[ i - 1 ] - hum_graph.min_value) * hum_unit );
      line_y1 = y_max - (int)round( (hum_graph.array_fGraph[ i ] - hum_graph.min_value) * hum_unit );
      if( line_y0 >= y_min && line_y0 <= y_max ) y0_ok = true;
      if( line_y1 >= y_min && line_y1 <= y_max ) y1_ok = true;

      if( y0_ok == true || y1_ok == true ){
        if( line_y0 < y_min ){
          line_y0 = y_min;
        }else if( line_y0 > y_max ){
          line_y0 = y_max;
        }
        if( line_y1 < y_min ){
          line_y1 = y_min;
        }else if( line_y1 > y_max ){
          line_y1 = y_max;
        }

        LCD.drawLine( line_x0, line_y0, line_x1, line_y1, hum_graph.red, hum_graph.green, hum_graph.blue );
        y0_ok = false, y1_ok = false;
      }
    }

    if( (gas_graph.array_fGraph[ i - 1 ] != 0) && (gas_graph.array_fGraph[ i ] != 0) ){
      line_y0 = y_max - (int)round( (gas_graph.array_fGraph[ i - 1 ] - gas_graph.min_value) * gas_unit );
      line_y1 = y_max - (int)round( (gas_graph.array_fGraph[ i ] - gas_graph.min_value) * gas_unit );
      if( line_y0 >= y_min && line_y0 <= y_max ) y0_ok = true;
      if( line_y1 >= y_min && line_y1 <= y_max ) y1_ok = true;

      if( y0_ok == true || y1_ok == true ){
        if( line_y0 < y_min ){
          line_y0 = y_min;
        }else if( line_y0 > y_max ){
          line_y0 = y_max;
        }
        if( line_y1 < y_min ){
          line_y1 = y_min;
        }else if( line_y1 > y_max ){
          line_y1 = y_max;
        }

        LCD.drawLine( line_x0, line_y0, line_x1, line_y1, gas_graph.red, gas_graph.green, gas_graph.blue );
        y0_ok = false, y1_ok = false;
      }
    }
  }

  m_isShowing_min_max = false;
}
//**********************************************
void DisplayBme680I2c::drawGraphNowAll( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, GraphParameter &gas_graph ){
  if( (m_graph_disp_ok == true) && (millis() - mp_plot_graph_last_time > m_graph_plot_period) ){
    long unsigned int now_time = now();
    if( now_time < 1532210272 ){
      now_time = 1532210272;
      setTime( (time_t)now_time );
    }

    switch( DispStatus ){
      case WriteDataError:
        //erro data
        pres_graph.array_fGraph[ pres_graph.x_data_max ] = -500;
        temp_graph.array_fGraph[ pres_graph.x_data_max ] = -500;
        hum_graph.array_fGraph[ pres_graph.x_data_max ] = -500;
        gas_graph.array_fGraph[ pres_graph.x_data_max ] = 0;
        break;
      case WriteGoodData:
        //Current value plot
        pres_graph.array_fGraph[ pres_graph.x_data_max ] = m_fPressure;
        temp_graph.array_fGraph[ pres_graph.x_data_max ] = m_fTemperature;
        hum_graph.array_fGraph[ pres_graph.x_data_max ] = m_fHumidity;
        gas_graph.array_fGraph[ pres_graph.x_data_max ] = m_fGas;

        m_array_time_plot[ pres_graph.x_data_max ] = now_time;

        break;
      default:
        break;
    }

    DisplayBme680I2c::drawGraphLineAll( pres_graph, temp_graph, hum_graph, gas_graph );

    int i;
    for( i = pres_graph.x_data_min; i <= pres_graph.x_data_max; i++ ){
      pres_graph.array_fGraph[ i - 1 ] = pres_graph.array_fGraph[ i ];
      temp_graph.array_fGraph[ i - 1 ] = temp_graph.array_fGraph[ i ];
      hum_graph.array_fGraph[ i - 1 ] = hum_graph.array_fGraph[ i ];
      gas_graph.array_fGraph[ i - 1 ] = gas_graph.array_fGraph[ i ];
      m_array_time_plot[ i - 1 ] = m_array_time_plot[ i ];
    }

    m_graph_disp_ok = false;
    m_isNewGraphPlotData = true;
    mp_plot_graph_last_time = millis();
  }
}
//********************************************
void DisplayBme680I2c::displayMinMaxValue( GraphParameter &pres_graph, GraphParameter &temp_graph, GraphParameter &hum_graph, GraphParameter &gas_graph, FontParameter &pres_font, FontParameter &temp_font, FontParameter &hum_font, FontParameter &gas_font ){
  if( (m_isShowing_min_max == true) && (millis() - mp_disp_min_max_last_time > 3000) ){
    DisplayBme680I2c::drawGraphLineAll( pres_graph, temp_graph, hum_graph, gas_graph ); //disp clear
    m_isShowing_min_max = false;
  }

  if( m_isDisp_min_max == true ){
    uint8_t font_buf[13][16] = {};
    uint16_t sj_length = 0;

    char pres_max_c[5], pres_min_c[5];
    char temp_max_c[4], temp_min_c[4];
    char hum_max_c[4], hum_min_c[4];
    char gas_max_c[7], gas_min_c[7];

    sprintf( pres_max_c, "%4d", (int)pres_graph.max_value );
    sprintf( pres_min_c, "%4d", (int)pres_graph.min_value );
    sprintf( temp_max_c, "%3d", (int)temp_graph.max_value );
    sprintf( temp_min_c, "%3d", (int)temp_graph.min_value );
    sprintf( hum_max_c, "%3d", (int)hum_graph.max_value );
    sprintf( hum_min_c, "%3d", (int)hum_graph.min_value );
    sprintf( gas_max_c, "%6d", (int)gas_graph.max_value );
    sprintf( gas_min_c, "%6d", (int)gas_graph.min_value );

    mgo_tec_esp32_bv1::FontParameter minmax_font;

    minmax_font.Xsize = 2;
    minmax_font.Ysize = 2;
    minmax_font.bg_red = 10, minmax_font.bg_green = 20, minmax_font.bg_blue = 10;

    int16_t xx = pres_graph.x_data_min + 16;
    int16_t y_max = pres_graph.y_data_min + 8;
    int16_t y_min = pres_graph.y_data_max - minmax_font.Ysize * 16 - 6;

    LCD.drawRectangleFill( xx - 8, y_max - 4, pres_graph.x_data_max - 8, y_max - 4 + minmax_font.Ysize * 16 + 4, minmax_font.bg_red, minmax_font.bg_green, minmax_font.bg_blue );
    LCD.drawRectangleFill( xx - 8, y_min - 4, pres_graph.x_data_max - 8, y_min - 4 + minmax_font.Ysize * 16 + 4, minmax_font.bg_red, minmax_font.bg_green, minmax_font.bg_blue );

    minmax_font.red = 31,  minmax_font.green = 63,  minmax_font.blue = 31;
    minmax_font.x0 = xx;
    minmax_font.y0 = y_max;
    sj_length = SFR.convStrToFont( "Max", font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );
    minmax_font.y0 = y_min;
    sj_length = SFR.convStrToFont( "Min", font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );

    minmax_font.red = pres_font.red,  minmax_font.green = pres_font.green,  minmax_font.blue = pres_font.blue;
    minmax_font.x0 = xx + 8 * minmax_font.Xsize * 4;
    minmax_font.y0 = y_max;
    sj_length = SFR.convStrToFont( String( pres_max_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );
    minmax_font.y0 = y_min;
    sj_length = SFR.convStrToFont( String( pres_min_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );

    minmax_font.Xsize = 1;
    minmax_font.red = temp_font.red,  minmax_font.green = temp_font.green,  minmax_font.blue = temp_font.blue;
    minmax_font.x0 = minmax_font.x0 + 8 * 2 * 5;
    minmax_font.y0 = y_max;
    sj_length = SFR.convStrToFont( String( temp_max_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );
    minmax_font.y0 = y_min;
    sj_length = SFR.convStrToFont( String( temp_min_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );

    minmax_font.red = hum_font.red,  minmax_font.green = hum_font.green,  minmax_font.blue = hum_font.blue;
    minmax_font.x0 = minmax_font.x0 + 8 * minmax_font.Xsize * 4;
    minmax_font.y0 = y_max;
    sj_length = SFR.convStrToFont( String( hum_max_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );
    minmax_font.y0 = y_min;
    sj_length = SFR.convStrToFont( String( hum_min_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );

    minmax_font.red = gas_font.red,  minmax_font.green = gas_font.green,  minmax_font.blue = gas_font.blue;
    minmax_font.x0 = minmax_font.x0 + 8 * minmax_font.Xsize * 4;
    minmax_font.y0 = y_max;
    sj_length = SFR.convStrToFont( String( gas_max_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );
    minmax_font.y0 = y_min;
    sj_length = SFR.convStrToFont( String( gas_min_c ) , font_buf );
    LCD.display8x16Font( minmax_font, sj_length, font_buf );

    m_isDisp_min_max = false;
    m_isShowing_min_max = true;
    mp_disp_min_max_last_time = millis();
  }
}
//********************************************
void DisplayBme680I2c::initDispValue( FontParameter &pres_font, FontParameter &temp_font, FontParameter &hum_font, int16_t x0, int16_t y0, char unit_c[][5] ){
  //red(0-31), green(0-63), blue(0-31)
  mgo_tec_esp32_bv1::FontParameter unit_font[3];

  uint16_t sj_length;
  uint8_t unit_font_buf[3][5][16] = {};
  unit_font[ 0 ].x0 = 64;
  unit_font[ 1 ].x0 = 208;
  unit_font[ 2 ].x0 = 288;

  int i;
  for( i = 0; i < 3; i++ ){
    unit_font[ i ].y0 = y0;
    unit_font[ i ].Xsize = 2, unit_font[ i ].Ysize = 2;
    unit_font[ i ].red = 31, unit_font[ i ].green = 63, unit_font[ i ].blue = 31;
    unit_font[ i ].bg_red = 0, unit_font[ i ].bg_green = 0, unit_font[ i ].bg_blue = 0;

    sj_length = SFR.convStrToFont( String( unit_c[ i ] ), unit_font_buf[ i ] );
    LCD.display8x16Font( unit_font[ i ], sj_length, unit_font_buf[ i ] );
  }

  pres_font.y0 = y0;
  temp_font.y0 = y0;
  hum_font.y0 = y0;

  pres_font.x0 = 0;
  temp_font.x0 = 128;
  hum_font.x0 = 240;
}
//********************************************
void DisplayBme680I2c::initDispGasValue( FontParameter &gas_font, int16_t x0, int16_t y0, char unit_c[5] ){
  //red(0-31), green(0-63), blue(0-31)
  mgo_tec_esp32_bv1::FontParameter unit_font;

  uint16_t sj_length;
  uint8_t unit_font_buf[7][16] = {};
  unit_font.x0 = x0 + 8 * gas_font.Xsize * 8;

  unit_font.y0 = y0;
  unit_font.Xsize = gas_font.Xsize, unit_font.Ysize = gas_font.Ysize;
  unit_font.red = 31, unit_font.green = 63, unit_font.blue = 31;
  unit_font.bg_red = 0, unit_font.bg_green = 0, unit_font.bg_blue = 0;

  sj_length = SFR.convStrToFont( String( unit_c ), unit_font_buf );
  LCD.display8x16Font( unit_font, sj_length, unit_font_buf );

  gas_font.y0 = y0;
  gas_font.x0 = x0;
}
//************************************************
void DisplayBme680I2c::displayValue( FontParameter &pres_font, FontParameter &temp_font, FontParameter &hum_font ){
  if( m_value_disp_ok == true ){
    uint8_t bme_font_buf[3][6][16] = {};
    uint16_t bme_sj_length = 0;

    if( mp_prev_pres_str != m_pres_str ){
      bme_sj_length = SFR.convStrToFont( m_pres_str, bme_font_buf[ 0 ] );
      LCD.display8x16Font( pres_font, bme_sj_length, bme_font_buf[ 0 ] );
      mp_prev_pres_str = m_pres_str;
    }
    if( mp_prev_temp_str != m_temp_str ){
      bme_sj_length = SFR.convStrToFont( m_temp_str, bme_font_buf[ 1 ] );
      LCD.display8x16Font( temp_font, bme_sj_length, bme_font_buf[ 1 ] );
      mp_prev_temp_str = m_temp_str;
    }
    if( mp_prev_hum_str != m_hum_str ){
      bme_sj_length = SFR.convStrToFont( m_hum_str, bme_font_buf[ 2 ] );
      LCD.display8x16Font( hum_font, bme_sj_length, bme_font_buf[ 2 ] );
      mp_prev_hum_str = m_hum_str;
    }

    m_value_disp_ok = false;
  }
}
//************************************************
void DisplayBme680I2c::displayGasValue( FontParameter &gas_font ){
  if( m_gas_value_disp_ok == true ){
    uint8_t bme_font_buf[9][16] = {};
    uint16_t bme_sj_length = 0;

    if( mp_prev_gas_str != m_gas_str ){
      bme_sj_length = SFR.convStrToFont( m_gas_str, bme_font_buf );
      LCD.display8x16Font( gas_font, bme_sj_length, bme_font_buf );
      mp_prev_gas_str = m_gas_str;
    }

    m_gas_value_disp_ok = false;
  }
}
//**********************************************
int32_t DisplayBme680I2c::calcGasAverageValue( GraphParameter &gas_graph, uint16_t sample ){
  if( sample > gas_graph.x_total_points ) sample = gas_graph.x_total_points; 
  uint16_t sample_point = gas_graph.x_data_max - sample;
  uint32_t dSum = 0;
  int i;
  for( i = sample_point; i <= gas_graph.x_data_max; i++ ){
    dSum += (uint32_t)gas_graph.array_fGraph[ i ];
  }
  return dSum / (gas_graph.x_data_max - sample_point + 1);
}
//**********************************
void DisplayBme680I2c::userDelayMs( uint32_t period ){
  delay(period);
}
//*********************************
int8_t bme680UserI2cRead( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len ){
  int8_t rslt_read = 0; /* Return 0 for Success, non-zero for failure */

  Wire.beginTransmission( dev_id );
  Wire.write( reg_addr );
  Wire.endTransmission();
  uint8_t req_from_ret = Wire.requestFrom((uint16_t)dev_id, (uint8_t)len, true);

  if( req_from_ret == 0 ){
    Serial.println("------- Error requestForm return 0 --------");
    rslt_read = 1;
  }else{
    for(int i=0; i<len; i++){
      reg_data[i] = Wire.read();
    }
    rslt_read = 0;
  }

  return rslt_read;
}
//******************************
int8_t bme680UserI2cWrite( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len ){
  int8_t rslt_write = 0; /* Return 0 for Success, non-zero for failure */

  Wire.beginTransmission( dev_id );
  Wire.write( reg_addr );
  for( int i = 0; i < len; i++ ){
    Wire.write( reg_data[i] );
  }
  rslt_write = Wire.endTransmission();

  return rslt_write;
}

}  // namespace mynamespace
