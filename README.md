# ESP32_mgo_tec
This is Arduino core for the ESP32 library Package by mgo-tec.  
Beta ver 1.0.3  
  
I renewed my past library and compiled it into one package.  
We modified function names and variable names as much as possible to general rules.   
  
Download and use the following library.  
- Time Library URL:  
https://github.com/PaulStoffregen/Time  
- BOSCH BME280 Driver URL:  
https://github.com/BoschSensortec/BME280_driver  
  
# Change log
(beta ver 1.0.3)  
- display_bme680_i2c.h Add a new & cpp file.  
- Added clearMsgWindow function to message_window file.  
Other minor modifications.  
  
(beta ver 1.0.2)  
- Added mgo_tec_m5stack_bme280.h file.  
- display_bme280_i2c.h Add & cpp file.  
Using the BOSCH BME 280 driver, create functions to display graphs and numerical values to ILI9341 such as M5Stack.  
BOSCH BME280 Driver URL:  
https://github.com/BoschSensortec/BME280_driver  
- Added the GraphParameter class to the ili9341_spi file.  
- Add black and white highlighted display to message_window file.  
- Other minor modifications.  
  
(beta ver 1.0.11)  
message_window.cpp  
Fix spelling error of dispWifiStatusMsg () function message.  
  
(beta ver 1.0.1)  
ili9341_spi.h  
ili9341_spi.cpp  
Fix LCD ILI9341 screen MAX size variable initialization.  
  
(beta ver 1.0)  
New Release.   
  
# 【更新履歴】(Japanese)
(beta ver 1.0.3)  
- display_bme680_i2c.h & cppファイルを新たに追加。  
- message_windowファイルに、clearMsgWindow関数追加。  
その他軽微な修正  
  
(beta ver 1.0.2)  
- mgo_tec_m5stack_bme280.hファイル追加。  
- display_bme280_i2c.h & cppファイルの追加。  
BOSCH BME280 ドライバを使い、M5Stack等のILI9341へグラフや数値を表示させる関数を作成。  
BOSCH BME280 Driver URL:  
https://github.com/BoschSensortec/BME280_driver  
- ili9341_spiファイルに、GraphParameterクラス追加。  
- message_windowファイルに、白黒反転表示を追加。  
- その他、軽微な修正多数。  
  
(beta ver 1.0.11)  
message_window.cpp  
dispWifiStatusMsg()関数のメッセージのスペルミスを修正。  
  
(beta ver 1.0.1)  
ili9341_spi.h  
ili9341_spi.cpp  
LCD ILI9341 画面MAXサイズ変数初期化を修正。  
  
(beta ver 1.0)  
新規リリース。  
過去の私の自作ライブラリを１つのパッケージにまとめました。  
そして、できるだけ一般的な命名規則に沿って関数名や変数名を変更してみました。  
初めての試みなので、いろいろ不具合あるかもしれません。  
いろいろ試行錯誤中です。  
  
The MIT License (MIT)  
MITライセンス  
Copyright (c) 2018 Mgo-tec  
  
webget.h  
LGPL-2.1  
  
My Blog:  
https://www.mgo-tec.com  