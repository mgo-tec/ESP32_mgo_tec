/*
  firebase_realtime_database.h
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
*/

#ifndef _MGO_TEC_ESP32_FIREBASEREALTIMEDATABASE_H_INCLUDED
#define _MGO_TEC_ESP32_FIREBASEREALTIMEDATABASE_H_INCLUDED

#include <Arduino.h>
#include <WiFiClientSecure.h>

// In the webget.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

class FirebaseRD
{
private:
  WiFiClientSecure client;
  const char *mp_host;
  const char *mp_auth;

public:
  enum FirebaseState { MsgReset, Connecting, ConnectOK, ConnectFailed }
    PatchStatus = MsgReset,
    SseStatus = MsgReset;
  int patch_status = 0, sse_status = 0;

  void init( const char *host, const char *auth );
  void sendGetRequestSSE( String path );
  void sendGetRequestSSE( const char *Root_Ca, uint8_t rca_set, String path );
  boolean receiveHttpResHeader();
  void patchHTTPrequest( String path, String target_node, String data_str );
  void patchHTTPrequest( const char *Root_Ca, uint8_t rca_set, String path, String target_node, String data_str );
  boolean pickUpStrSSEdataAll( String &str );
  boolean pickUpStrToTargetStr( String resp_str, String node_str, String &target_str );
  boolean pickUpTargetStr( String node_str, String &target_str );

};

}// namespace mgo_tec_esp32_bv1

#endif
