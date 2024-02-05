# ESP32_mgo_tec
This is Arduino core for the ESP32 library Package by mgo-tec.  
Beta ver 1.0.83  
  
I renewed my past library and compiled it into one package.  
We modified function names and variable names as much as possible to general rules.   
  
Download and use the following library.  
- Time Library URL:  
https://github.com/PaulStoffregen/Time  
- BOSCH BME280 Driver URL:  
https://github.com/BoschSensortec/BME280_driver  
- BOSCH BME680 Driver URL:  
https://github.com/BoschSensortec/BME680_driver  
  
If the BOSCH sensor library is not installed, a compile error will be issued.  
If you do not need it, delete /Sensor/ folder and the files in it.  
  
※BOSCH純正BME280,及び BME680ドライバライブラリをインストールされていない場合はコンパイルエラーが出ます。  
その場合、/Sensor/フォルダを削除してください。  
  
# Change log
(beta ver 1.0.83)  
Fixed an issue in yahooj_rss.cpp where a rainstorm icon would appear when it was snowing..  
  
(beta ver 1.0.82)  
In yahooj_rss.cpp, there was an unused constant and I got a compilation error, so I deleted it.  
  
(beta ver 1.0.81)  
The color of the weather forecast icon was partially incorrect, so I corrected it.
  
(beta ver 1.0.80)  
Updated so that the weather forecast can be obtained from the Japan Meteorological Agency website.  
  
(beta ver 1.0.72)  
- Fixed the problem that Yahoo Japan RSS weather forecast is not displayed when the string "later" is used.  
(yahooj_rss.cpp file modified)  
  
(beta ver 1.0.71)  
- The display error when the ESP32 development board and the ILI9341 module were connected by SPI was corrected.  
- Slightly fixed NTP server related.  
  
(beta ver 1.0.70)  
Support for the latest M5Stack IPS display (ILI9342C).  
The first M5Stack ILI9341 is automatically detected.  
Thanks to [lovyan] (https://twitter.com/lovyan03) for teaching me.  
Modify the following files.  
- ili9341_spi.h  
- ili9341_spi.cpp  
  
(beta ver 1.0.69)  
- Added drawPixel65kColRGB565Bytes function to ili9341_spi.cpp file.  
RGB565 image data can be sent at once and displayed on the LCD.  
- Added init function and init_button function separately from button operation function from init function in mgo_tec_bv1_m5stack_sd_init.cpp file.  
  
(beta ver 1.0.67)  
Added function that can input RGB color with value 0-255.  
- Added colorRGB255, bgColorRGB255 function to FontParameter class.  
- Added drawRectangleFill255 function to ILI9341Spi class.  
  
(beta ver 1.0.65)  
- Firebase related libraries have been added and fixed.  
- Other, minor fixes.  
  
(beta ver 1.0.60)  
- It is possible to input HTML color code for graphic display and character display.  
- Other minor corrections.  
  
(beta ver 1.0.51)  
Added the dispInversionOn and dispInversionOff functions in the ili9341spi.h and ili9341spi.cpp files.
  
(beta ver 1.0.50)  
- Fully corrected.  
- Character display and scroll display program in M5Stack is now easier.  
See sample sketch.  
- By pressing the button of M5Stack it made it possible to display select box.  
- Brushed up the Yahoo news display and simplified it on the sketch.  
  
(beta ver 1.0.40)  
- Added firebase_realtime_database.h & cpp file.  
- Add display_shinonome_fnt. & cpp file.  
I tried scrolling the Japanese Shinonome font string easily.  
- Fix scrolle8x16font function of ili9341spi.cpp. Other minor fixes.  
- Fix convSjisToFontInc function of shinonome.cpp.  
- Minor modification of message_window.cpp.  
- Fix scrolleYahooJnews function in yahooj_rss.cpp.  
- Added file mgo_tec_esp32_m5stack_firebase.h file.  
  
(beta ver 1.0.31)  
Modified dispMsgWindow function in message_window.cpp file slightly.  
  
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
(beta ver 1.0.83)  
yahooj_rss.cpp で、雪の場合に暴風雨アイコンが出てしまうのを修正しました。  
  
(beta ver 1.0.82)  
yahooj_rss.cpp で、未使用の定数があり、コンパイルエラーが出たので消去しました。  
  
(beta ver 1.0.81)  
天気予報アイコンの色が一部誤っていたので修正しました。  
  
(beta ver 1.0.80)  
Yahoo RSS 天気予報が2022/03/31で配信終了してしまったため、気象庁ホームページから天気予報を取得できるように修正。  
[https://www.mgo-tec.com/blog-entry-jp-weather01.html](https://www.mgo-tec.com/blog-entry-jp-weather01.html)  
  
(beta ver 1.0.72)  
- Yahoo Japan RSS 天気予報表示で、「のち」という文字列の場合表示されない問題を修正しました。（yahooj_rss.cpp ファイル修正）  
  
(beta ver 1.0.71)  
- ESP32開発ボードとILI9341モジュールをSPI接続した場合の表示異常を修正しました。  
- NTPサーバー関連を少々修正。  
  
(beta ver 1.0.70)  
最新のM5StackのIPSディスプレイ (ILI9342C) に対応しました。  
初代M5Stack ILI9341とは自動判別するようにしました。  
教えて頂いた[らびやん](https://twitter.com/lovyan03)さんに感謝です。  
以下のファイルを修正。  
- ili9341_spi.h  
- ili9341_spi.cpp  
  
(beta ver 1.0.69)  
- ili9341_spi.cppファイルにdrawPixel65kColRGB565Bytes関数を追加。  
RGB565画像データを一気に送ってLCDに表示できるようにしました。  
- mgo_tec_bv1_m5stack_sd_init.cppファイルのinit関数からボタン操作関数と分離してinit関数とinit_button関数を新たに追加ました。  
  
(beta ver 1.0.67)  
RGB の色を0-255の値で入力できる関数を追加。  
- FontParameterクラスにcolorRGB255, bgColorRGB255関数追加。  
- ILI9341SpiクラスにdrawRectangleFill255関数追加。  
  
(beta ver 1.0.65)  
- Firebase 関連のライブラリを追加および修正しました。  
- その他、軽微な修正。  
  
(beta ver 1.0.60)  
- グラフィック表示および文字表示に HTML カラーコード入力ができるようにしました。  
- その他軽微な修正。  
  
(beta ver 1.0.51)  
- ili9341_spi.h および ili9341_spi.cpp ファイルの dispInversionOn および dispInversionOff 関数を追加。  
  
(beta ver 1.0.50)  
- 全面的に修正  
- M5Stack で文字表示やスクロール表示プログラムが簡単になりました。  
sampleスケッチ参照。  
- M5Stack のボタンを押すとセレクトボックス表示できるようにしました。  
- Yahoo ニュース表示をブラッシュアップし、スケッチ上でシンプル化しました。  
  
(beta ver 1.0.40)  
- firebase_realtime_database.h & cpp ファイル追加。  
- display_shinonome_fnt. & cpp ファイル追加。  
日本語東雲フォント文字列のスクロールを簡単にしてみました。  
- ili9341_spi.cpp の、scrolle8x16font関数を修正。その他、軽微な修正。  
- shinonome.cpp の convSjisToFontInc関数を修正。  
- message_window.cpp の軽微な修正。  
- yahooj_rss.cpp の scrolleYahooJnews関数修正。  
- mgo_tec_esp32_m5stack_firebase.h ファイル追加。  
  
(beta ver 1.0.31)    
message_window.cpp ファイルのdispMsgWindow関数を少々修正しました。  
  
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