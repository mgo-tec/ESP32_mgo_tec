/*
  firebase_realtime_database.cpp
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

#include "ESP32_mgo_tec_bV1/Web/firebase_realtime_database.h"

// In the firebase_realtime_database.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.
  
//**********************************************************
void FirebaseRD::init( const char *host, const char *auth ){
  mp_host = host;
  mp_auth = auth;
}

//***************** SSL https GET *************************************************
void FirebaseRD::sendGetRequestSSE( String path ){
  FirebaseRD::sendGetRequestSSE( "", 0, path );
}

void FirebaseRD::sendGetRequestSSE( const char *Root_Ca, uint8_t rca_set, String path ){
  FirebaseRD::SseStatus = Connecting;
  sse_status = Connecting;
  if( rca_set ==1 ){
    client.setCACert(Root_Ca);
    Serial.println(F("-------Root CA SET"));
  }else{
    Serial.println(F("------- Nothing Root CA"));
  }

  uint32_t time_out = millis();
  while( 1 ){
    /*インターネットが不意に切断されたときや、長時間接続している時には再接続できなくなる。
    再接続時、client.connect が true になるまで時間がかかる場合があるので、数回トライする必要がある。*/
    if ( client.connect( mp_host, 443 ) ){
      Serial.println( F("Connected to server!") );
      String req_url_str;
      req_url_str = F("GET /");
      req_url_str += path + F(".json?auth=");
      req_url_str += String( mp_auth ) + F(" HTTP/1.1\r\n");

      String req_header_str;
      req_header_str = F("Host: ");
      req_header_str += String( mp_host ) + F("\r\n");
      req_header_str += F("Accept: text/event-stream\r\n");
      req_header_str += F("Connection: close\r\n");
      req_header_str += F("\r\n");

      Serial.println( F("----- Send GET Request Server-Sent Events") );
      client.print( req_url_str );
      client.print( req_header_str );
      client.flush(); //client出力が終わるまで待つ

      log_v( "%s", req_url_str.c_str() );
      log_v( "%s", req_header_str.c_str() );
      //Serial.print( req_url_str );
      //Serial.print( req_header_str );
      //Serial.flush(); //シリアル出力が終わるまで待つ指令は、余分なdelayがかかってしまうので基本的に使わない
      break;
    }

    if( ( millis() - time_out ) > 20000 ){
      FirebaseRD::SseStatus = ConnectFailed;
      sse_status = ConnectFailed;
      Serial.println( F("time out!") );
      Serial.println( F("Host connection failed.") );
      return;
    }
    delay(1);
  }

  time_out = millis();
  while( client.connected() ){
    if( (millis() - time_out) > 60000 ){
      log_v( "time out" );
      FirebaseRD::SseStatus = ConnectFailed;
      sse_status = ConnectFailed;
      break;
    } 
    if( receiveHttpResHeader() ){
      FirebaseRD::SseStatus = ConnectOK;
      sse_status= ConnectOK;
      break;
    }
  }
}
//*******************************************
boolean FirebaseRD::receiveHttpResHeader(){
  while ( client.available() ) {
    String resp_str = client.readStringUntil('\n');
    if( resp_str.length() > 0 ){
      Serial.println( F("----- Received HTTP Responce Header -----") );
      if( resp_str.indexOf( "HTTP/1.1 200 OK" ) >= 0 ){
        Serial.println( resp_str );
        while( client.available() ){
          resp_str = client.readStringUntil('\n');
          if( resp_str == "\r" ) break;
          if( resp_str.length() > 0 ) Serial.println( resp_str );
        }
        Serial.println( F("----- Received body -----") );
        return true;
      }else{
        while ( client.available() ) {
          char c = client.read();
          Serial.write( c );
        }
      }
    }
  }
  return false;
}
//******Firebase Realtime Database 書き替え**************
void FirebaseRD::patchHTTPrequest( String path, String target_node, String data_str ){
  FirebaseRD::patchHTTPrequest( "", 0, path, target_node, data_str );
}

void FirebaseRD::patchHTTPrequest( const char *Root_Ca, uint8_t rca_set, String path, String target_node, String data_str ){
  if( client ){
    delay(2);
    client.stop(); //Server-Sent Events受信中は、一旦client.stop()しないと、Firebase 側がPUTを受け付けない。
    delay(2); //このdelayは必要
    FirebaseRD::SseStatus = MsgReset;
    sse_status = MsgReset;
  }

  if( rca_set ==1 ){
    client.setCACert(Root_Ca);
    Serial.println(F("-------Root CA SET"));
  }else{
    Serial.println(F("------- Nothing Root CA"));
  }

  PatchStatus = Connecting;
  patch_status = Connecting;
  uint32_t time_out = millis();
  while( 1 ){
    /*インターネットが不意に切断されたときや、長時間接続している時には再接続できなくなる。
    再接続時、client.connect が true になるまで時間がかかる場合があるので、数回トライする必要がある。*/
    if( client.connect( mp_host, 443 ) ){
      Serial.println( F("Connected to server!") );
      String req_url_str;
      req_url_str = F("PATCH /");
      req_url_str += path;
      req_url_str += F(".json?auth=");
      req_url_str += String( mp_auth );
      req_url_str += F(" HTTP/1.1\r\n");

      String body = "{\"" + target_node + "\":\"" + data_str + "\"}";
      body += F("\r\n\r\n");

      String head;
      head = F("Host: ");
      head += String(mp_host) + F("\r\n");
      head += F("Connection: close\r\n");
      head += F("Content-Length: ");
      head += String( body.length() ) + F("\r\n\r\n");

      client.print( req_url_str );
      client.print( head );
      client.println( body );
      client.flush(); //client出力が終わるまで待つ

      Serial.println( F("----- Send HTTP Patch Request") );
      log_v( "%s", req_url_str.c_str() );
      log_v( "%s", head.c_str() );
      log_v( "%s", body.c_str() );
      //Serial.print( req_url_str );
      //Serial.print( head );
      //Serial.print( body );
      break;
    }

    if( ( millis() - time_out ) > 20000 ){
      PatchStatus = ConnectFailed;
      patch_status = ConnectFailed;
      Serial.println( F("time out!") );
      Serial.println( F("Host connection failed.") );
      return;
    }
    delay(1);
  }

  time_out = millis();
  while( client.connected() ){
    if( (millis() - time_out) > 60000 ){
      log_v( "time out" );
      PatchStatus = ConnectFailed;
      patch_status = ConnectFailed;
      break;
    } 
    if( receiveHttpResHeader() ){
      //Received body
      while ( client.available() ) {
        char c = client.read();
        Serial.write( c );
      }
      Serial.println( F("\r\n-------------------------") );
      delay(2); //Patchリクエストしたら、必ずclient.stopしないとServer-Sent Events受信できない。
      client.stop();
      delay(10);
      PatchStatus = ConnectOK;
      patch_status = ConnectOK;
      sendGetRequestSSE( Root_Ca, rca_set, path );
      break;
    }
  }
}
//***************************************
boolean FirebaseRD::pickUpStrSSEdataAll( String &str ){
  str = "";
  if( FirebaseRD::SseStatus == ConnectOK ){
    String resp_str;
    while( client.available() ){
      resp_str = client.readStringUntil('\n');
      if( resp_str.length() > 0 ){
        Serial.println( resp_str.c_str() ); //while文の中ではlog_v出力できない。
        if( resp_str.indexOf( "data:" ) == 0 ){
          str = resp_str;
          return true;
        }
      }
    }
  }
  return false;
}
//***************************************
boolean FirebaseRD::pickUpStrToTargetStr( String resp_str, String node_str, String &target_str ){
  int str_index = resp_str.indexOf( node_str + "\"" );
  if( str_index > 0 ){
    int target_index0 = resp_str.indexOf( ":\"", str_index ) + 2;
    int target_index1 = resp_str.indexOf( "\"", target_index0 );
    target_str = resp_str.substring( target_index0, target_index1 );
    log_v( "target_str = %s", target_str.c_str() );
    return true;
  }
  return false;
}
//**************************************
//※これは１つのデータしか取得できない。
//Realtime Database に初回接続した時、全てのデータを取得しても、そのうちの target データしか取得できないことに注意
boolean FirebaseRD::pickUpTargetStr( String node_str, String &target_str ){
  String resp_str;
  if( FirebaseRD::pickUpStrSSEdataAll( resp_str ) ){
    return FirebaseRD::pickUpStrToTargetStr( resp_str, node_str, target_str );
  }
  return false;
}
//********** HTML Color Code str to 65k color value *****************************************
void FirebaseRD::pickUpColorData( String color_str, uint8_t &Red, uint8_t &Green, uint8_t &Blue ){
  String red_str = color_str.substring( 1, 3 );
  String green_str = color_str.substring( 3, 5 );
  String blue_str = color_str.substring( 5, 7 );
  uint8_t red_value = strtol( red_str.c_str(), NULL, 16 );
  uint8_t green_value = strtol( green_str.c_str(), NULL, 16 );
  uint8_t blue_value = strtol( blue_str.c_str(), NULL, 16 );
  Red = (uint8_t)floor( (double)red_value / 8.0 );
  Green = (uint8_t)floor( (double)green_value / 4.0 );
  Blue = (uint8_t)floor( (double)blue_value / 8.0 );
}


}  // namespace mynamespace
