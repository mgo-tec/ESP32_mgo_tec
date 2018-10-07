/*
  mgo_tec_bv1_m5stack_sd_yahoo_init1.h
  Beta version 1.0.0
*/

#ifndef MGO_TEC_BV1_M5STACK_SD_YAHOO_INIT1_H_INCLUDED_
#define MGO_TEC_BV1_M5STACK_SD_YAHOO_INIT1_H_INCLUDED_

#include "mgo_tec_bv1_m5stack_sd_init.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/watch.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/yahooj_rss.h"
#include "ESP32_mgo_tec_bV1/Display/ILI9341_SD/message_window.h"
#include "ESP32_mgo_tec_bV1/Switch/button_select_box.h"

// In the mgo_tec_bv1_m5stack_sd_yahoo_init1.h file
namespace mgo_tec_esp32_bv1 {

// All declarations are within the namespace scope.
// Notice the lack of indentation.

class MgotecM5stackSdYahooInit1 : public MgotecM5stackSdInit
{
public:
  ILI9341Watch watch;
  MessageWindow msg, wifi_msg;
  YahooJrssGet yahoo[4];
};

}// namespace mgo_tec_esp32_bv1

#endif
