/*
  mgo_tec_m5stack_firebase_sd_init1.h
  This is an include-only file for linking other files.

  beta ver 1.0.0

  Use ESP32, LCD ILI9341 spi, SD spi
*/

#ifndef MGO_TEC_M5STACK_FIREBASE_SD_INI1_H_INCLUDED_
#define MGO_TEC_M5STACK_FIREBASE_SD_INI1_H_INCLUDED_

#include "mgo_tec_bv1_m5stack_sd_init.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/watch.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/message_window.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/display_shinonome_fnt.h"
#include "ESP32_mgo_tec_bV1/Switch/button_switch.h"
#include "ESP32_mgo_tec_bV1/Web/firebase_realtime_database.h"

// In the mgo_tec_bv1_m5stack_sd_yahoo_init1.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

class MgotecM5stackFirebaseSdInit1 : public MgotecM5stackSdInit
{
public:
  FontParameter font;
  ScrolleParameter scl_set;
  ILI9341Watch watch;
  DispShinonomeFnt disp_fnt;
  MessageWindow msg, wifi_msg, sse_msg, patch_msg;
  FirebaseRD firebase;
};

}// namespace mgo_tec_esp32_bv1

#endif
