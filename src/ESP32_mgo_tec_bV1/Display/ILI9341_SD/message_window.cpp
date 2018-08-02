/*
  message_window.cpp - for Arduino core for the ESP32.
  ( Use LCD ILI9341 and SD )
  Beta version 1.0.2
  
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
*/

#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/message_window.h"

// In the message_window.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.

//*********************************************
void MessageWindow::dispWifiStatusMsg(){
  switch( WifiStatus ){
    case WifiConnecting:
      font.red = 31;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 30;
      m_bg_blue = 15;
      MessageWindow::dispMsgWindow(1000, "WiFi Connecting...");
      WifiStatus = WifiMsgReset;
      break;
    case WifiConnected:
      font.red = 0;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 0;
      m_bg_green = 30;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1001, "WiFi Connected!");
      WifiStatus = WifiMsgReset;
      break;
    case WifiFailed:
      font.red = 31;
      font.green = 0;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1002, "WiFi Failed");
      WifiStatus = WifiMsgReset;
      break;
    default:
      break;
  }
}

//*********************************************
/* Wifi.status()
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
*/
void MessageWindow::dispWifiStatusMsg2(){
  switch( WiFi.status() ){
    case WL_IDLE_STATUS:
      MessageWindow::dispMsgWindow(1100, "WL_IDLE_STATUS");
      break;
    case WL_NO_SSID_AVAIL:
      MessageWindow::dispMsgWindow(1101, "WL_NO_SSID_AVAIL");
      break;
    case WL_SCAN_COMPLETED:
      MessageWindow::dispMsgWindow(1102, "WL_SCAN_COMPLETED");
      break;
    case WL_CONNECTED:
      MessageWindow::dispMsgWindow(1103, "WL_CONNECTED");
      break;
    case WL_CONNECT_FAILED:
      MessageWindow::dispMsgWindow(1104, "WL_CONNECT_FAILED");
      break;
    case WL_CONNECTION_LOST:
      MessageWindow::dispMsgWindow(1105, "WL_CONNECTION_LOST");
      break;
    case WL_DISCONNECTED:
      MessageWindow::dispMsgWindow(1106, "WL_DISCONNECTED");
      break;
    default:
      break;
  }
}
//*********************************************
void MessageWindow::dispWifiStatusMsgShort(){
  switch( WifiStatus ){
    case WifiConnecting:
      font.red = 31;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 30;
      m_bg_blue = 15;
      MessageWindow::dispMsgWindow(1000, "WiFi？");
      WifiStatus = WifiMsgReset;
      break;
    case WifiConnected:
      font.red = 0;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 0;
      m_bg_green = 30;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1001, "WiFi◎");
      WifiStatus = WifiMsgReset;
      break;
    case WifiFailed:
      font.red = 31;
      font.green = 0;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1002, "WiFi×");
      WifiStatus = WifiMsgReset;
      break;
    default:
      break;
  }
}
//*********************************************
void MessageWindow::dispWebGetStatusMsgShort( int &web_status, String str ){
  switch( web_status ){
    case Connecting:
      font.red = 31;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 30;
      m_bg_blue = 15;
      MessageWindow::dispMsgWindow(2000, str + "??");
      web_status = MsgReset;
      break;
    case ConnectOK:
      font.red = 0;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 0;
      m_bg_green = 30;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2001, str + "◎");
      web_status = MsgReset;
      break;
    case ConnectFailed:
      font.red = 31;
      font.green = 0;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2002, str + "×");
      web_status = MsgReset;
      break;
    default:
      break;
  }
}

//*********************************************
void MessageWindow::dispWebGetStatusMsgLong( uint8_t &web_status, String str ){
  switch( web_status ){
    case Connecting:
      font.red = 31;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 30;
      m_bg_blue = 15;
      MessageWindow::dispMsgWindow(2100, str + " Connecting");
      web_status = MsgReset;
      break;
    case ConnectOK:
      font.red = 0;
      font.green = 63;
      font.blue = 0;
      m_bg_red = 0;
      m_bg_green = 30;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2101, str + " Connect OK!");
      web_status = MsgReset;
      break;
    case ConnectFailed:
      font.red = 31;
      font.green = 0;
      font.blue = 0;
      m_bg_red = 15;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2102, str + " Failed");
      web_status = MsgReset;
      break;
    default:
      break;
  }
}
//*********白黒反転************************************
void MessageWindow::dispWifiStatusMsgRev(){
  switch( WifiStatus ){
    case WifiConnecting:
      m_bg_red = 20;
      m_bg_green = 40;
      m_bg_blue = 20;
      MessageWindow::dispMsgWindow(1000, "WiFi Connecting...");
      WifiStatus = WifiMsgReset;
      break;
    case WifiConnected:
      m_bg_red = 0;
      m_bg_green = 50;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1001, "WiFi Connected!");
      WifiStatus = WifiMsgReset;
      break;
    case WifiFailed:
      m_bg_red = 25;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1002, "WiFi Failed");
      WifiStatus = WifiMsgReset;
      break;
    default:
      break;
  }
}
//*********************************************
void MessageWindow::dispWifiStatusMsgShortRev(){
  switch( WifiStatus ){
    case WifiConnecting:
      m_bg_red = 20;
      m_bg_green = 40;
      m_bg_blue = 20;
      MessageWindow::dispMsgWindow(1000, "WiFi？");
      WifiStatus = WifiMsgReset;
      break;
    case WifiConnected:
      m_bg_red = 0;
      m_bg_green = 40;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1001, "WiFi◎");
      WifiStatus = WifiMsgReset;
      break;
    case WifiFailed:
      m_bg_red = 20;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(1002, "WiFi×");
      WifiStatus = WifiMsgReset;
      break;
    default:
      break;
  }
}
//********白黒反転*************************************
void MessageWindow::dispWebGetStatusMsgShortRev( int &web_status, String str ){
  switch( web_status ){
    case Connecting:
      m_bg_red = 20;
      m_bg_green = 40;
      m_bg_blue = 20;
      MessageWindow::dispMsgWindow(2000, str + "??");
      web_status = MsgReset;
      break;
    case ConnectOK:
      m_bg_red = 0;
      m_bg_green = 40;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2001, str + "◎");
      web_status = MsgReset;
      break;
    case ConnectFailed:
      m_bg_red = 20;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2002, str + "×");
      web_status = MsgReset;
      break;
    default:
      break;
  }
}
//********白黒反転*************************************
void MessageWindow::dispWebGetStatusMsgLongRev( uint8_t &web_status, String str ){
  switch( web_status ){
    case Connecting:
      m_bg_red = 20;
      m_bg_green = 40;
      m_bg_blue = 20;
      MessageWindow::dispMsgWindow(2100, str + " Connecting");
      web_status = MsgReset;
      break;
    case ConnectOK:
      m_bg_red = 0;
      m_bg_green = 40;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2101, str + " Connect OK!");
      web_status = MsgReset;
      break;
    case ConnectFailed:
      m_bg_red = 20;
      m_bg_green = 0;
      m_bg_blue = 0;
      MessageWindow::dispMsgWindow(2102, str + " Failed");
      web_status = MsgReset;
      break;
    default:
      break;
  }
}
//*****************************************************
void MessageWindow::dispMsgWindow( int16_t msg_num, String str ){
  if( msg_num != m_prev_msg_num ){
    if( m_txt_length > 40 ) m_txt_length = 40; //最大半角40文字
    uint16_t X1 = m_x0 + m_padding * 2 + (m_txt_length * 8) * m_size - 1;
    if( X1 > LCD.m_max_pix_x1){
      X1 = LCD.m_max_pix_x1;
    }
    uint16_t Y1 = m_y0 + m_padding * 2 + m_size * 16 - 1;
    if( Y1 > LCD.m_max_pix_y1){
      Y1 = LCD.m_max_pix_y1;
    }

    uint8_t f_buf[ str.length() ][ 16 ] = {};
    uint16_t len = SFR.convStrToFont(str, f_buf);
    if( len > m_txt_length ) len = m_txt_length;

    font.Xsize = m_size;
    font.Ysize = m_size;
    font.x0 = m_x0 + m_padding;
    font.y0 = m_y0 + m_padding;
    font.bg_red = m_bg_red;
    font.bg_green = m_bg_green;
    font.bg_blue = m_bg_blue;

    LCD.drawRectangleFill( m_x0, m_y0, X1, Y1, m_bg_red, m_bg_green, m_bg_blue );

    LCD.display8x16Font( font, len, f_buf );
    LCD.drawRectangleLine( m_x0, m_y0, X1, Y1, m_line_red, m_line_green, m_line_blue );
    m_prev_msg_num = msg_num;
    //Serial.printf( "message_window no.%d, (%s)\r\n", msg_num, str.c_str() );
  }
}
//*****************************************************
void MessageWindow::clearMsgWindow( int16_t msg_num ){
  //表示されたメッセージ番号(msg_num)と合うものだけクリアーする
  if( msg_num == m_prev_msg_num ){
    uint16_t X1 = m_x0 + m_padding * 2 + (m_txt_length * 8) * m_size - 1;
    if( X1 > LCD.m_max_pix_x1){
      X1 = LCD.m_max_pix_x1;
    }
    uint16_t Y1 = m_y0 + m_padding * 2 + m_size * 16 - 1;
    if( Y1 > LCD.m_max_pix_y1){
      Y1 = LCD.m_max_pix_y1;
    }
    LCD.drawRectangleFill( m_x0, m_y0, X1, Y1, m_bg_red, m_bg_green, m_bg_blue );
    m_prev_msg_num = -1;
  }
}

}  // namespace mynamespace
