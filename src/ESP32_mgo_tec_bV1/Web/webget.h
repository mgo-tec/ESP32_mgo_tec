/*
  WebGet.h
  Beta version 2.0.0

Copyright (c) 2018 Mgo-tec

This library is used by the Arduino IDE(Tested in ver1.8.2).

Reference Blog --> https://www.mgo-tec.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Reference LGPL-2.1 license statement --> https://opensource.org/licenses/LGPL-2.1   

WiFi.h - Included WiFi library for esp32
Based on WiFi.h from Arduino WiFi shield library.
Copyright (c) 2011-2014 Arduino.  All right reserved.
Modified by Ivan Grokhotkov, December 2014
Licensed under the LGPL-2.1

WiFiClientSecure.h
Copyright (c) 2011 Adrian McEwen.  All right reserved.
Additions Copyright (C) 2017 Evandro Luis Copercini.
Licensed under the LGPL-2.1

WiFiUdp.h
Copyright (c) 2008 Bjoern Hartmann
Licensed under the MIT.

Use Arduino Time Library ( TimeLib.h )
time.c - low level time and date functions
Copyright (c) Michael Margolis 2009-2014
LGPL ver2.1
https://github.com/PaulStoffregen/Time

*/

#ifndef MGO_TEC_ESP32_BV1_WEBGET_H_INCLUDED_
#define MGO_TEC_ESP32_BV1_WEBGET_H_INCLUDED_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <TimeLib.h> //Arduino time library ver1.5-

// In the webget.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

//****************************
class WebGetClass
{
private:
  uint32_t mp_LastNTPtime = 0;
  IPAddress mp_NtpServerIP;
  static const int mp_NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
  byte mp_packetBuffer[ mp_NTP_PACKET_SIZE ]; //buffer to hold incoming and outgoing packets
  WiFiUDP mp_Udp;
  int mp_timeZone = 9; // Tokyo

public:
  bool wifiAPconnect( const char *ssid, const char *password );
  void ntpServerInit( int timezone, const char *NtpServerName );
  void ntpServerInit( int timezone, const char *NtpServerName, const uint16_t local_port );
  void getNtpTimeInit( int timezone, const char *NtpServerName );

  String webGet( const char* host0, String target_ip, char char_tag, String Final_tag, String Begin_tag, String End_tag, String Paragraph );
  String httpsWebGet( const char* host1, String target_ip, char char_tag, String Final_tag, String Begin_tag, String End_tag, String Paragraph );
  String httpsWebGet( const char *root_ca, const char* host1, String target_ip, char char_tag, String Final_tag, String Begin_tag, String End_tag, String Paragraph );
  String httpsGet( const char *Root_Ca, uint8_t rca_set, const char* Host, String target_url, char c_tag, String F_tag, String B_tag, String E_tag, String Pph );

  String httpsGet( const char *Root_Ca, uint8_t rca_set, const char* Host, const uint16_t Port, String target_url, char c_tag, String F_tag, String B_tag, String E_tag, String Pph );

  String getJapanWeatherPartJson(
    const char *Root_Ca,
    uint8_t rca_set,
    const char* Host,
    const uint16_t Port,
    String target_url,
    char separation_tag,
    String search_key,
    String paragraph);

  bool getNtpServerSelect( uint8_t timezone );
  void getNtpInterval( uint32_t interval );
  
  time_t getNtpTime();
  void sendNtpPacket( IPAddress &address );

};

}// namespace mgo_tec_esp32_bv1

#endif
