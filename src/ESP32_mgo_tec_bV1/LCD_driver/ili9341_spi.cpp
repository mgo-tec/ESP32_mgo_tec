/*
  ili9341_spi.cpp - for Arduino core for the ESP32 ( Use SPI library ).
  Beta version 1.0.6
  ESP32_LCD_ILI9341_SPI library class has been redesigned.
  
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

Modify Display.cpp of M5stack library.
M5stack library - MIT License
Copyright (c) 2017 M5Stack
*/

#include "ESP32_mgo_tec_bV1/LCD_driver/ili9341_spi.h"

// In the ili9341_spi.cpp file
namespace mgo_tec_esp32_bv1 {

// Definition of functions is within scope of the namespace.

//****** LCD ILI9341 ディスプレイ初期化 ***********
void ILI9341Spi::ILI9341init( int8_t sck, int8_t miso, int8_t mosi, int8_t cs, int8_t dc, int8_t rst, int8_t led, uint32_t clk, bool use_hwcs ){
  //LCD と micro SD カードを共用の VSPI で使う場合は use_hwcs=falseにする。
  //SPIFFS で使う場合は use_hwcs=true にする
  ILI9341Spi::brightness( 0 );

  mp_sck = sck;
  mp_miso = miso;
  mp_mosi = mosi;
  mp_cs = cs;
  mp_dc = dc;
  mp_rst = rst;
  mp_ledpin = led;

  mp_useHw_Cs = use_hwcs; //VSPI Hardware CS 
  mp_Freq = clk;

  pinMode( mp_rst, OUTPUT ); //Set RESET pin
  pinMode( mp_dc, OUTPUT ); //Set Data/Command pin

  SPI.begin( mp_sck, mp_miso, mp_mosi, mp_cs ); //VSPI setting

  SPI.setBitOrder( MSBFIRST );
  //ILI9341 のSPI Clock Cycle Time (Write) Minimun 100ns=10MHz
  SPI.setFrequency( clk );
  SPI.setDataMode( SPI_MODE0 );
  SPI.setHwCs( mp_useHw_Cs ); //Set Hardware CS pin

  //Hardware Reset------------
  digitalWrite( mp_rst, HIGH );
  delay( 5 );
  digitalWrite( mp_rst, LOW );
  delay( 10 );
  digitalWrite( mp_rst, HIGH );
  delay( 121 );

  if( !mp_useHw_Cs ){
    pinMode( mp_cs, OUTPUT );
    digitalWrite( mp_cs, HIGH );
    digitalWrite( mp_cs, LOW );
  }
  digitalWrite( mp_dc, HIGH );

  ILI9341Spi::commandWrite( 0x38 ); //Idle mode OFF
  ILI9341Spi::commandWrite( 0x3A ); //COLMOD: Pixel Format Set
    ILI9341Spi::dataWrite( 0b01010101 ); //RGB 16 bits / pixel, MCU 16 bits / pixel
  ILI9341Spi::commandWrite( 0x36 ); //MADCTL: Memory Access Control
    ILI9341Spi::dataWrite( 0b0001000 ); //M5stack only. D3: BGR(RGB-BGR Order control bit )="1"
    //ILI9341Spi::dataWrite( 0b00101000 ); //サインスマート 2.2インチ用。D3: BGR(RGB-BGR Order control bit )="1"
  ILI9341Spi::commandWrite( 0x11 ); //Sleep OUT
  delay(10);

  ILI9341Spi::commandWrite( 0x29 ); //Display ON

  ILI9341Spi::displayClear( 0, 0, 319, 239 );

  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
  ILI9341Spi::brightness( 100 );
}
//********* Display Rotation ***************************
void ILI9341Spi::dispRotation( uint8_t rot ){
  uint8_t b = 0b00001000;
  switch( rot ){
    case 0: //M5stack 横表示、デフォルト
      b = 0b00001000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 1: //M5stack 縦表示、デフォルト
      b = 0b10101000;
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    case 2: //M5stack 横表示、上下逆
      b = 0b11001000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 3: //M5stack 縦表示、上下逆
      b = 0b01101000;
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    //------------------------
    case 4: //M5stack 横表示、左右反転
      b = 0b01001000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 5: //M5stack 縦表示、左右反転
      b = 0b00101000;
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    case 6: //M5stack 横表示、上下逆、左右反転
      b = 0b10001000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 7: //M5stack 縦表示、上下逆、左右反転
      b = 0b11101000;
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    //-------------------------------
    case 8: //M5stack 横表示、デフォルト、上下反転
      b = 0b10001000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 9: //M5stack 縦表示、デフォルト、上下反転
      b = 0b11101000;
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    case 10: //M5stack 横表示、上下逆、上下反転
      b = 0b01001000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 11: //M5stack 縦表示、上下逆、上下反転
      b = 0b00101000;
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    //-------------------------------
    case 12: //M5stack で表示変わらず
      b = 0b00001100;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 13: //M5stack で表示変わらず
      b = 0b00011000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;

    //------------------------
    case 250:
      b = 0b00101000; //HiLetgo 2.8", サインスマート販売のILI9341横正常表示
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 251:
      b = 0b10001000; //HiLetgo 2.8", サインスマート販売のILI9341の縦方向表示
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    case 252: //HiLetgo 2.8"　横表示　上下逆
      b = 0b11101000;
      m_max_disp_width = mp_max_disp_width;
      m_max_disp_height = mp_max_disp_height;
      break;
    case 253: //HiLetgo 2.8"、 縦表示　上下逆
      b = 0b01001000;
      m_max_disp_width = mp_max_disp_height;
      m_max_disp_height = mp_max_disp_width;
      break;
    default:
      break;
  }
  m_max_pix_x1 = m_max_disp_width - 1;
  m_max_pix_y1 = m_max_disp_height - 1;
  m_max_font8x16_x = m_max_disp_width / 8;
  m_max_font8x16_y = m_max_disp_height / 16;
  ILI9341Spi::spiSetChange();
  ILI9341Spi::commandWrite( 0x36 ); //MADCTL: Memory Access Control
    ILI9341Spi::dataWrite( b ); //M5stack only. D3: BGR(RGB-BGR Order control bit )="1"
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//********* Color bit Inversion OFF ************
void ILI9341Spi::dispInversionOn(){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::commandWrite( 0x21 ); //MADCTL: Memory Access Control
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//********* Color bit Inversion ON ************
void ILI9341Spi::dispInversionOff(){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::commandWrite( 0x20 ); //MADCTL: Memory Access Control
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//********* 4wire SPI Data / Command write************
void ILI9341Spi::commandWrite( uint8_t b ){
  digitalWrite( mp_dc, LOW );
  SPI.write( b );
  digitalWrite( mp_dc, HIGH );
}

void ILI9341Spi::dataWrite( uint8_t b ){
  SPI.write(b);
}

void ILI9341Spi::dataWrite16( uint16_t b ){
  SPI.write16(b);
}

void ILI9341Spi::dataWrite32( uint32_t b ){
  SPI.write32(b);
}

void ILI9341Spi::dataWriteBytes( uint8_t *b, uint32_t b_size ){
  SPI.writeBytes( b, b_size );
}
//**************************************************
void ILI9341Spi::spiSetChange(){
  SPI.setFrequency( mp_Freq );
  SPI.setDataMode( SPI_MODE0 );
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, LOW );
}
//******** Set Column and Page Address ( X Y range setting )***********
void ILI9341Spi::rangeXY( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 ){
  uint32_t X = (uint32_t)x0 << 16 | x1;
  uint32_t Y = (uint32_t)y0 << 16 | y1;

  ILI9341Spi::commandWrite( 0x2A ); //Set Column Address
    ILI9341Spi::dataWrite32( X );
  ILI9341Spi::commandWrite( 0x2B ); //Set Page Address
    ILI9341Spi::dataWrite32( Y );
}
//********* Display All Black Clear ******************************
void ILI9341Spi::displayClear(){
  ILI9341Spi::displayClear( 0, 0, m_max_pix_x1, m_max_pix_y1 );
}
//********* Display All Black Clear ******************************
void ILI9341Spi::displayClear( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 ){
  if( x0 > m_max_pix_x1 ) x0 = m_max_pix_x1;
  if( y0 > m_max_pix_y1 ) y0 = m_max_pix_y1;
  if( x1 > m_max_pix_x1 ) x1 = m_max_pix_x1;
  if( y1 > m_max_pix_y1 ) y1 = m_max_pix_y1;
  uint32_t Total_Pixels = ( x1 - x0 + 1 ) * ( y1 - y0 + 1 ) ;
  ILI9341Spi::blockSPIfastWrite( x0, y0, x1, y1, 0, 0, 0, Total_Pixels );
}
//*********** LCD ILE9341 Block Pixel SPI Fast Write *****************
void ILI9341Spi::blockSPIfastWrite( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue, uint32_t repeat ){
  ILI9341Spi::spiSetChange();
  uint16_t ColorDot = ( red << 11 ) | ( green << 5 ) | blue;
  ILI9341Spi::rangeXY( x0, y0, x1, y1 );
  ILI9341Spi::commandWrite( 0x2C ); //LCD RAM write
  ILI9341Spi::spiWriteBlock( ColorDot, repeat );
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//********* Display Color Pixel Block Fast Write *****************
void ILI9341Spi::spiWriteBlock(uint16_t color, uint32_t repeat) {
  uint16_t color16 = ( color >> 8 ) | ( color << 8 );
  uint32_t color32 = color16 | color16 << 16;

  if ( repeat > 15 ) {
    SET_PERI_REG_BITS( SPI_MOSI_DLEN_REG( mp_spi_num ), SPI_USR_MOSI_DBITLEN, 255,
                      SPI_USR_MOSI_DBITLEN_S );

    while ( repeat > 15 ) {
      while ( READ_PERI_REG( SPI_CMD_REG( mp_spi_num ) ) & SPI_USR )
        ;
      for ( uint32_t i = 0; i < 16; i++ )
        WRITE_PERI_REG( ( SPI_W0_REG( mp_spi_num ) + ( i << 2 ) ), color32 );
      SET_PERI_REG_MASK( SPI_CMD_REG( mp_spi_num ), SPI_USR );
      repeat -= 16;
    }
    while ( READ_PERI_REG( SPI_CMD_REG( mp_spi_num ) ) & SPI_USR )
      ;
  }

  if ( repeat ) {
    repeat = ( repeat << 4 ) - 1;
    SET_PERI_REG_BITS( SPI_MOSI_DLEN_REG( mp_spi_num ), SPI_USR_MOSI_DBITLEN, repeat,
                      SPI_USR_MOSI_DBITLEN_S );
    for ( uint32_t i = 0; i < 16; i++ )
      WRITE_PERI_REG( ( SPI_W0_REG( mp_spi_num ) + ( i << 2 ) ), color32 );
    SET_PERI_REG_MASK( SPI_CMD_REG( mp_spi_num ), SPI_USR );
    while ( READ_PERI_REG( SPI_CMD_REG( mp_spi_num ) ) & SPI_USR )
      ;
  }
}
//*********** 65k Color Pixel write bytes ****************************
void ILI9341Spi::drawPixel65kColRGB565Bytes( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *b, uint32_t b_size ){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::rangeXY( x0, y0, x1, y1 );
  ILI9341Spi::commandWrite( 0x2C ); //RAM write
  SPI.writeBytes( b, b_size );
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//*********** 65k Color Pixel (Dot) Write ( SD card use )*************
void ILI9341Spi::drawPixel65kDotColor_sd( uint16_t x0, uint16_t y0, uint16_t DotColor ){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::drawPixel65kDotColor( x0, y0, DotColor);
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//*********** HTML Color Code 65k Pixel RGB color Write ( SD card use )************
void ILI9341Spi::drawPixel65k3Color_sd( uint16_t x0, uint16_t y0, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawPixel65k3Color_sd( x0, y0, red, green, blue );
}
//*********** 65k Pixel RGB color Write ( SD card use )****************************
void ILI9341Spi::drawPixel65k3Color_sd( uint16_t x0, uint16_t y0, uint8_t red, uint8_t green, uint8_t blue ){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::drawPixel65k3Color( x0, y0, red, green, blue );
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//*********** 65k Color Pixel (Dot) Write  ( SPIFFS use ) ****************************
void ILI9341Spi::drawPixel65kDotColor( uint16_t x0, uint16_t y0, uint16_t DotColor ){
  ILI9341Spi::rangeXY( x0, y0, x0, y0 );
  ILI9341Spi::commandWrite( 0x2C ); //RAM write
  ILI9341Spi::dataWrite16( DotColor );
}
//*********** HTML Color Code 65k Pixel RGB color Write ( SPIFFS use )************
void ILI9341Spi::drawPixel65k3Color( uint16_t x0, uint16_t y0, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawPixel65k3Color( x0, y0, red, green, blue );
}
//*********** 65k Pixel RGB color Write ( SPIFFS use ) ***************************
void ILI9341Spi::drawPixel65k3Color( uint16_t x0, uint16_t y0, uint8_t red, uint8_t green, uint8_t blue ){
  //red (0-31), green (0-63), blue (0-31)
  ILI9341Spi::rangeXY( x0, y0, x0, y0 );

  uint16_t Dot = ( (uint16_t)red << 11 ) | ( (uint16_t)green << 5 ) | (uint16_t)blue;

  ILI9341Spi::commandWrite( 0x2C ); //RAM write
  ILI9341Spi::dataWrite16( Dot );
}
//***************************************
void ILI9341Spi::drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawLine( x0, y0, x1, y1, red, green, blue);
}
//***************************************
void ILI9341Spi::drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t red, uint8_t green, uint8_t blue ){
  ILI9341Spi::spiSetChange();
  int i;
  int16_t Y = 0, X = 0;
  int16_t length_x = x1 - x0;
  int16_t length_y = y1 - y0;

  uint16_t Dot = ( red << 11 ) | ( green << 5 ) | blue;

  if( abs( length_x ) > abs( length_y ) ){
    float degY = ( (float)length_y ) / ( (float)length_x );
    if( x0 < x1 ){
      for( i = x0; i < ( x1 + 1 ); i++ ){
        Y = y0 + round( ( i - x0 ) * degY );
        ILI9341Spi::drawPixel65kDotColor( i, Y, Dot );
      }
    }else{
      for( i = x0; i >= x1; i-- ){
        Y = y0 + round( ( i - x0 ) * degY );
        ILI9341Spi::drawPixel65kDotColor( i, Y, Dot );
      }
    }
  }else{
    float degX = ( (float)length_x ) / ( (float)length_y );
    if( y0 < y1 ){
      for( i = y0; i < ( y1 + 1); i++ ){
        X = x0 + round( ( i - y0 ) * degX );
        ILI9341Spi::drawPixel65kDotColor( X, i, Dot );
      }
    }else{
      for( i = y0; i >= y1; i-- ){
        X = x0 + round( ( i - y0 ) * degX );
        ILI9341Spi::drawPixel65kDotColor( X, i, Dot );
      }
    }
  }
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//***************************************
void ILI9341Spi::drawHorizontalLine( int16_t x0, int16_t x1, int16_t y0, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawHorizontalLine( x0, x1, y0, red, green, blue );
}
//***************************************
void ILI9341Spi::drawHorizontalLine( int16_t x0, int16_t x1, int16_t y0, uint8_t red, uint8_t green, uint8_t blue ){
  if( x1 < x0 ) std::swap( x0, x1 );
  uint32_t Width_x = x1 - x0 + 1;
  ILI9341Spi::blockSPIfastWrite( x0, y0, x1, y0, red, green, blue, Width_x );
}
//***************************************
void ILI9341Spi::drawVerticallLine( int16_t x0, int16_t y0, int16_t y1, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawVerticallLine( x0, y0, y1, red, green, blue );
}
//***************************************
void ILI9341Spi::drawVerticallLine( int16_t x0, int16_t y0, int16_t y1, uint8_t red, uint8_t green, uint8_t blue ){
  if( y1 < y0 ) std::swap( y0, y1 );
  uint16_t Width_y = y1 - y0 + 1;
  ILI9341Spi::blockSPIfastWrite( x0, y0, x0, y1, red, green, blue, Width_y );
}
//***************************************
void ILI9341Spi::drawRectangleLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawRectangleLine( x0, y0, x1, y1, red, green, blue );
}
//***************************************
void ILI9341Spi::drawRectangleLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue ){
  ILI9341Spi::drawHorizontalLine( x0, x1, y0, red, green, blue );
  ILI9341Spi::drawHorizontalLine( x0, x1, y1, red, green, blue );
  ILI9341Spi::drawVerticallLine( x0, y0, y1, red, green, blue );
  ILI9341Spi::drawVerticallLine( x1, y0, y1, red, green, blue );
}
//***************************************
void ILI9341Spi::drawRectangleFill( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawRectangleFill( x0, y0, x1, y1, red, green, blue );
}
//***************************************
void ILI9341Spi::drawRectangleFill( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue ){
  uint16_t Width_x = x1 - x0 + 1;
  uint16_t Width_y = y1 - y0 + 1;
  uint32_t Total = Width_x * Width_y ;
  ILI9341Spi::blockSPIfastWrite( x0, y0, x1, y1, red, green, blue, Total );
}
//**********red(0-255), green(0-255), blue(0-255)************
void ILI9341Spi::drawRectangleFill255( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t red, uint8_t green, uint8_t blue ){
  uint16_t Width_x = x1 - x0 + 1;
  uint16_t Width_y = y1 - y0 + 1;
  uint32_t Total = Width_x * Width_y;
  uint8_t tmp_red = (uint8_t)floor( (double)red / 8.0 );
  uint8_t tmp_green = (uint8_t)floor( (double)green / 4.0 );
  uint8_t tmp_blue = (uint8_t)floor( (double)blue / 8.0 );
  ILI9341Spi::blockSPIfastWrite( x0, y0, x1, y1, tmp_red, tmp_green, tmp_blue, Total );
}
//***************************************
void ILI9341Spi::drawCircleLine( uint16_t x0, uint16_t y0, uint16_t r, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawCircleLine( x0, y0, r, red, green, blue );
}
//***************************************
void ILI9341Spi::drawCircleLine( uint16_t x0, uint16_t y0, uint16_t r, uint8_t red, uint8_t green, uint8_t blue ){
  ILI9341Spi::spiSetChange();
  uint16_t x1, y1;
  float i;
  float deg = 1.0;
  if( r > 50 ) deg = 0.5;
  if( r > 110) deg = 0.25;

  uint16_t Dot = ( (uint16_t)red << 11 ) | ( (uint16_t)green << 5 ) | (uint16_t)blue;
  int16_t dummy_x = -1, dummy_y = -1;

  for( i = 0; i < 360; i = i + deg ){
    x1 = round( (float)( x0 + ( r * cos( radians(i) ) ) ) );
    y1 = round( (float)( y0 + ( r * sin( radians(i) ) ) ) );

    if( ( dummy_x != x1 ) || ( dummy_y != y1 ) ){
      ILI9341Spi::drawPixel65kDotColor( x1, y1, Dot );
      dummy_x = x1;
      dummy_y = y1;
    }
  }
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//***************************************
void ILI9341Spi::drawCircleFill( uint16_t x0, uint16_t y0, uint16_t r, String html_color_code ){
  uint8_t red = 0, green = 0, blue = 0;
  mp_hccc.convHtmlColCodeTo65kCol( html_color_code, red, green, blue );
  ILI9341Spi::drawCircleFill( x0, y0, r, red, green, blue );
}
//***************************************
void ILI9341Spi::drawCircleFill( uint16_t x0, uint16_t y0, uint16_t r, uint8_t red, uint8_t green, uint8_t blue ){
  //red (0-31), green (0-63), blue (0-31)
  uint16_t x1, y1;
  float i;
  float deg = 1.0;
  //半径が大きくなると、角度の刻み方を細かくしないと、完全に塗りつぶせないので注意。
  if( r > 50 ) deg = 0.5;
  if( r > 110) deg = 0.25;

  int16_t dummy_x = -1, dummy_y = -1;
  for( i = 0; i < 360; i = i + deg ){
    x1 = round( (float)( x0 + ( r * cos( radians( i ) ) ) ) );
    y1 = round( (float)( y0 + ( r * sin( radians( i ) ) ) ) );
    if( ( dummy_x != x1 ) || ( dummy_y != y1 ) ){
      ILI9341Spi::drawVerticallLine( x1, y0, y1, red, green, blue );
      dummy_x = x1;
      dummy_y = y1;
    }
  }
}
//********* LCD Idle Mode OFF ***********************
void ILI9341Spi::idleModeOff(){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::commandWrite( 0x38 ); //Idle mode OFF
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//********* LCD Idle Mode ON ************************
void ILI9341Spi::idleModeOn(){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::commandWrite( 0x39 ); //Idle mode ON
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//********* LCD Display LED brightness **************
void ILI9341Spi::brightness( uint8_t brightness ){
  uint8_t ledc_ch = 0;
  uint32_t valueMax = 255;
  uint32_t duty = ( 8191 / valueMax ) * brightness;
  ledcSetup( ledc_ch, 5000, 13 );
  ledcAttachPin( mp_ledpin, 0 );
  ledcWrite( ledc_ch, duty );
}
//************************************************
//******** 以下、フォント関数 ********************
//************************************************
uint8_t ILI9341Spi::XscrolleFontArrayInit( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_width, uint8_t Xsize, uint8_t Ysize){
  scl_set.txt_width = txt_width;
  scl_set.txt_height = 1;
  font.Xsize = Xsize;
  font.Ysize = Ysize;
  font.prev_Xsize = Xsize;
  font.prev_Ysize = Ysize;

  ILI9341Spi::fontParamMaxClip( font, scl_set );  
  scl_set.heap_array_size1 = scl_set.txt_width * 8 * font.Xsize * 2;
  scl_set.heap_array_size2 = 16;
  log_v( "X.heap_array_size1=%d, ary_size2=%d", scl_set.heap_array_size1, scl_set.heap_array_size2 );

  return ILI9341Spi::array2newCreate( scl_set, scl_set.heap_array_size1, scl_set.heap_array_size2 );
}
//********************************************************************
uint8_t ILI9341Spi::XscrolleFontArrayInitMax( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_width, uint8_t Xsize, uint8_t Ysize){
  scl_set.txt_width = txt_width; 
  scl_set.txt_height = 1;
  font.Xsize = Xsize;
  font.Ysize = Ysize;
  font.prev_Xsize = Xsize;
  font.prev_Ysize = Ysize;

  ILI9341Spi::fontParamMaxClip( font, scl_set );
  txt_width = 40; //最大配列サイズにする
  scl_set.heap_array_size1 = txt_width * 8 * font.Xsize * 2;
  scl_set.heap_array_size2 = 16;

  return ILI9341Spi::array2newCreate( scl_set, scl_set.heap_array_size1, scl_set.heap_array_size2 );
}
//********************************************************************
uint8_t ILI9341Spi::YscrolleFontArrayInit( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_height, uint8_t Xsize, uint8_t Ysize){
  scl_set.txt_width = 2;
  scl_set.txt_height = txt_height;
  font.Xsize = Xsize;
  font.Ysize = Ysize;
  font.prev_Xsize = Xsize;
  font.prev_Ysize = Ysize;
  //txt_heightは少数を切り上げて多めに領域確保

  ILI9341Spi::fontParamMaxClip( font, scl_set );
  scl_set.heap_array_size1 = 16 * font.Xsize * 2;
  scl_set.heap_array_size2 = 16 * scl_set.txt_height;
  log_v( "Y.heap_array_size1=%d, ary_size2=%d", scl_set.heap_array_size1, scl_set.heap_array_size2 );

  return ILI9341Spi::array2newCreate( scl_set, scl_set.heap_array_size1, scl_set.heap_array_size2 );
}
//********************************************************************
uint8_t ILI9341Spi::YscrolleFontArrayInitMax( FontParameter &font, ScrolleParameter &scl_set, uint16_t txt_height, uint8_t Xsize, uint8_t Ysize){
  scl_set.txt_width = 2;
  scl_set.txt_height = txt_height;
  font.Xsize = Xsize;
  font.Ysize = Ysize;
  font.prev_Xsize = Xsize;
  font.prev_Ysize = Ysize;

  ILI9341Spi::fontParamMaxClip( font, scl_set );
  txt_height = 15; //最大配列サイズにする
  scl_set.heap_array_size1 = 16 * font.Xsize * 2;
  scl_set.heap_array_size2 = 16 * txt_height;

  return ILI9341Spi::array2newCreate( scl_set, scl_set.heap_array_size1, scl_set.heap_array_size2 );
}
//********************************************************************
uint8_t ILI9341Spi::array2newCreate( ScrolleParameter &scl_set, uint16_t ary_size1, uint16_t ary_size2){
  if( scl_set.isHeap_create == true ){
    log_v( "Cannot create new heap memory.\r\n heap size = %d", esp_get_free_heap_size() );
    ILI9341Spi::scrolleArrayDelete( scl_set );
  }
  //log_v("Before Create New Free Heap Size = %d", esp_get_free_heap_size());

  //ヒープ領域２次元配列動的確保
  scl_set.heap_array = new uint8_t *[ ary_size2 ];
  //if(scl_set.heap_array == NULL) return 0;
  int i, j;
  for( i = 0; i < ary_size2; i++ ) {
    scl_set.heap_array[ i ] = new uint8_t [ ary_size1 ];
    //if(scl_set.heap_array[ i ] == NULL) return 0;
  }

  for( i = 0; i < ary_size2; i++ ){
    for( j = 0; j < ary_size1; j++ ){
      scl_set.heap_array[i][j] = 0;
    }
  }

  scl_set.isHeap_create = true;
  log_v( "After Create New Free Heap Size = %d", esp_get_free_heap_size() );
  return 1;
}
//********************************************************************
void ILI9341Spi::scrolleArrayDelete( ScrolleParameter &scl_set ){
  if( scl_set.isHeap_create == false ){
    log_v( "Cannot create delete heap memory.\r\n heap size = %d", esp_get_free_heap_size() );
    log_v( "First, you should create heap memory!" );
    return;
  }
  //log_v("Before Delete Free Heap Size = %d", esp_get_free_heap_size());

  for ( int i = 0; i < scl_set.heap_array_size2; i++ ){
    delete[] scl_set.heap_array[i];
  }
  delete[] scl_set.heap_array;

  scl_set.isHeap_create = false;
  log_v( "After Delete Free Heap Size = %d", esp_get_free_heap_size() );
}
//********************************************************************
void ILI9341Spi::fontParamMaxClip( FontParameter &font, ScrolleParameter &scl_set ){
  if( font.x0 >= m_max_disp_width || font.x0 < 0 ) font.x0 = 0;
  if( font.y0 >= m_max_disp_height || font.y0 < 0 ) font.y0 = 0;

  scl_set.max_disp_x0_x1 = m_max_disp_width - font.x0;
  if( scl_set.max_disp_x0_x1 < font.Xsize * 8 ){
    font.x0 = 0;
    scl_set.max_disp_x0_x1 = m_max_disp_width;
  }
  scl_set.max_disp_y0_y1 = m_max_disp_height - font.y0;
  if( scl_set.max_disp_y0_y1 < font.Ysize * 16 ){
    font.y0 = 0;
    scl_set.max_disp_y0_y1 = m_max_disp_height;
  }

  if( font.Xsize >= m_max_font8x16_x ){
    font.Xsize = m_max_font8x16_x;
  }
  if( font.Ysize >= m_max_font8x16_y ){
    font.Ysize = m_max_font8x16_y;
  }

  //scl_set.txt_widthは少数を下げて文字欠けを防ぐ
  uint8_t txt_wd_max = floor( (float)scl_set.max_disp_x0_x1 / ( 8 * font.Xsize ) );
  if( scl_set.txt_width > txt_wd_max ){
    scl_set.txt_width = txt_wd_max;
  }
  uint8_t txt_ht_max = floor( (float)scl_set.max_disp_y0_y1/ ( 16 * font.Ysize ) );
  if( scl_set.txt_height > txt_ht_max ){
    scl_set.txt_height = txt_ht_max;
  }

  scl_set.x_pixel_size = scl_set.txt_width * ( 8 * font.Xsize );
  if( scl_set.x_pixel_size > scl_set.max_disp_x0_x1 ){
    scl_set.x_pixel_size = scl_set.max_disp_x0_x1;
  }
  scl_set.y_pixel_size = scl_set.txt_height * ( font.Ysize * 16 );
  if( scl_set.y_pixel_size > scl_set.max_disp_y0_y1 ){
    scl_set.y_pixel_size = scl_set.max_disp_y0_y1;
  }

  if( scl_set.x_scl_send_bytes > scl_set.heap_array_size1 ){
    scl_set.x_scl_send_bytes = scl_set.heap_array_size1;
  }
}
//********************************************************************
void ILI9341Spi::fontParamMaxClip( FontParameter &font ){
  if( font.x0 >= m_max_disp_width || font.x0 < 0 ) font.x0 = 0;
  if( font.y0 >= m_max_disp_height || font.y0 < 0 ) font.y0 = 0;

  font.max_disp_x0_x1 = m_max_disp_width - font.x0;
  if( font.max_disp_x0_x1 < font.Xsize * 8 ){
    font.x0 = 0;
    font.max_disp_x0_x1 = m_max_disp_width;
  }
  font.max_disp_y0_y1 = m_max_disp_height - font.y0;
  if( font.max_disp_y0_y1 < font.Ysize * 16 ){
    font.y0 = 0;
    font.max_disp_y0_y1 = m_max_disp_height;
  }

  if( font.Xsize >= m_max_font8x16_x ){
    font.Xsize = m_max_font8x16_x;
  }
  if( font.Ysize >= m_max_font8x16_y ){
    font.Ysize = m_max_font8x16_y;
  }

  //font.txt_widthは少数を下げて文字欠けを防ぐ
  uint8_t txt_wd_max = floor( (float)font.max_disp_x0_x1 / ( 8 * font.Xsize ) );
  if( font.txt_width > txt_wd_max ){
    font.txt_width = txt_wd_max;
  }
  uint8_t txt_ht_max = floor( (float)font.max_disp_y0_y1/ ( 16 * font.Ysize ) );
  if( font.txt_height > txt_ht_max ){
    font.txt_height = txt_ht_max;
  }

  font.x_pixel_size = font.txt_width * ( 8 * font.Xsize );
  if( font.x_pixel_size > font.max_disp_x0_x1 ){
    font.x_pixel_size = font.max_disp_x0_x1;
  }
  font.y_pixel_size = font.txt_height * ( font.Ysize * 16 );
  if( font.y_pixel_size > font.max_disp_y0_y1 ){
    font.y_pixel_size = font.max_disp_y0_y1;
  }
}
//********************************************************************
void ILI9341Spi::scrolleFontSetUp( FontParameter &font, ScrolleParameter &scl_set, bool xy_range_set ){  
  ILI9341Spi::scrolleFontColorSet( font );
  ILI9341Spi::scrolleFormSet( font, scl_set, scl_set.txt_width, scl_set.txt_height, font.Xsize, font.Ysize);

  if( xy_range_set ){
    ILI9341Spi::spiSetChange();
    ILI9341Spi::rangeXY( font.x0, font.y0, font.x0 + scl_set.x_pixel_size - 1, font.y0 + scl_set.y_pixel_size - 1 );
    ILI9341Spi::commandWrite( 0x2C ); //RAM write
    if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
  }
  //Serial.printf("--------------------scl_set.x_pixel_size= %d\r\n", scl_set.x_pixel_size);
  //Serial.printf("--------------------scl_set.y_pixel_size= %d\r\n", scl_set.y_pixel_size);
}
//********************************************************************
void ILI9341Spi::scrolleFontColorSet( FontParameter &font ){
  font.dot_msb = ( font.red << 3) | ( font.green >> 3 );
  font.dot_lsb = ( font.green << 5) | font.blue;

  font.bg_dot_msb = ( font.bg_red << 3 ) | ( font.bg_green >> 3 );
  font.bg_dot_lsb = ( font.bg_green << 5 ) | font.bg_blue;
}
//********************************************************************
void ILI9341Spi::scrolleFormSet( FontParameter &font, ScrolleParameter &scl_set, uint8_t txt_width, uint8_t txt_height, uint8_t Xsize, uint8_t Ysize ){
  scl_set.x_pixel_size = txt_width * ( 8 * Xsize );
  scl_set.y_pixel_size = txt_height * ( Ysize * 16 );
  font.Xsize = Xsize;
  font.Ysize = Ysize;
  font.prev_Xsize = Xsize;
  font.prev_Ysize = Ysize;

  ILI9341Spi::fontParamMaxClip( font, scl_set );

  scl_set.y_scl_send_bytes = font.Xsize * 16 * 2;
}
//********************************************************************
void ILI9341Spi::scrolleXYrangeComChange( FontParameter &font, ScrolleParameter &scl_set ){
  ILI9341Spi::spiSetChange();
  ILI9341Spi::rangeXY( font.x0, font.y0, font.x0 + scl_set.x_pixel_size - 1, font.y0 + scl_set.y_pixel_size - 1 );
  ILI9341Spi::commandWrite( 0x2C ); //RAM write
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}
//********************************************************************
void ILI9341Spi::scrolle8x16font( FontParameter &font, ScrolleParameter &scl_set, uint16_t *fnt_cnt, const uint16_t &font_sjis_len, uint8_t Fnt[][16], bool xy_range_set){
  //Default xy_range_set = true;
  if( millis() - scl_set.scl_last_time > scl_set.interval){
    if( ( font.Xsize != font.prev_Xsize ) || ( font.Ysize != font.prev_Ysize ) ){
      ILI9341Spi::scrolleFormSet( font, scl_set,  scl_set.txt_width, scl_set.txt_height, font.Xsize, font.Ysize );
    }

    ILI9341Spi::spiSetChange();

    if( xy_range_set ){
      ILI9341Spi::rangeXY( font.x0, font.y0, font.x0 + scl_set.x_pixel_size - 1, font.y0 + scl_set.y_pixel_size - 1 );
      ILI9341Spi::commandWrite( 0x2C ); //RAM write
    }

    int i, j;
    uint16_t inc_ary;
    uint8_t bt1 = 0b10000000;
    uint16_t array_max = scl_set.x_pixel_size * 2;

    for( i = 0; i < 16; i++ ){
      inc_ary = scl_set.x_scl_send_bytes;
      if( Fnt[ *fnt_cnt ][ i ] & ( bt1 >> scl_set.single_fnt_scl_cnt ) ){
        for( j = font.Xsize; j > 0; j-- ){
          scl_set.heap_array[ i ][ inc_ary++ ] = font.dot_msb;
          scl_set.heap_array[ i ][ inc_ary++ ] = font.dot_lsb;
          if(inc_ary >= array_max) break;
        }
      }else{
        for( j = font.Xsize; j > 0; j-- ){
          scl_set.heap_array[ i ][ inc_ary++ ] = font.bg_dot_msb;
          scl_set.heap_array[ i ][ inc_ary++ ] = font.bg_dot_lsb;
          if(inc_ary >= array_max) break;
        }
      }
    }

    if( ( inc_ary + 2 * font.Xsize ) > array_max ) inc_ary = 0;

    int16_t now_dec_send_byteNum = array_max - inc_ary;
//Serial.printf("arry_max=%d, inc_ary=%d, now_dec_send_byteNum=%d\r\n",array_max, inc_ary, now_dec_send_byteNum);
//Serial.printf("scl_cnt=%d, fnt_cnt=%d\r\n", scl_set.single_fnt_scl_cnt, *fnt_cnt);
    for( i = 0; i < 16; i++ ){
      for( j = font.Ysize; j > 0; j-- ){
        ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][ inc_ary ], now_dec_send_byteNum );
        ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][ 0 ], inc_ary );
      }
    }

    scl_set.x_scl_send_bytes = inc_ary;

    scl_set.single_fnt_scl_cnt++;
    if( scl_set.single_fnt_scl_cnt > 7 ){
      scl_set.single_fnt_scl_cnt = 0;
      (*fnt_cnt)++;
      if( *fnt_cnt >= font_sjis_len ) {
        *fnt_cnt = 0;
      }
    }

    scl_set.scl_last_time = millis();
    if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
  }
}
//********************************************************************
boolean ILI9341Spi::scrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set ){
  boolean ret = false;
  scl_set.font_sjis_len = fontSJ_Length;
  //半角２文字分スクロールし、次のフォントを毎回読み出すようにする。
  ILI9341Spi::scrolle8x16font( font, scl_set, &scl_set.full_or_half_cnt, scl_set.font_sjis_len, Fnt, xy_range_set );
  if( ( scl_set.full_or_half == 2 ) && ( scl_set.full_or_half_cnt == 2 ) ){
    scl_set.full_or_half_cnt = 0;
    ret = true;
  }else if( ( scl_set.full_or_half == 1 ) && ( scl_set.full_or_half_cnt == 1 ) ){
    scl_set.full_or_half_cnt = 0;
    ret = true;
  }
  return ret;
}
//********************************************************************
void ILI9341Spi::reverseScrolle8x16font( FontParameter &font, ScrolleParameter &scl_set, uint16_t *fnt_cnt, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set ){
  //Default xy_range_set = true;
  //半角２文字連続の場合は、東雲フォントライブラリで2文字一まとめの全角として扱われるので、逆スクロールの場合は読み取れなくなる。
  //しかし、基本的に半角英語の逆スクロールは無いものと考え、敢えて修正しない。
  if( millis() - scl_set.scl_last_time > scl_set.interval){
    scl_set.font_sjis_len = fontSJ_Length;
    if( ( font.Xsize != font.prev_Xsize ) || ( font.Ysize != font.prev_Ysize ) ){
      ILI9341Spi::scrolleFormSet( font, scl_set,  scl_set.txt_width, scl_set.txt_height, font.Xsize, font.Ysize );
    }
    ILI9341Spi::spiSetChange();

    if( xy_range_set ){
      ILI9341Spi::rangeXY( font.x0, font.y0, font.x0 + scl_set.x_pixel_size - 1, font.y0 + scl_set.y_pixel_size - 1 );
      ILI9341Spi::commandWrite( 0x2C ); //RAM write
    }

    int i, j;
    uint8_t bt1 = 0b00000001;
    int dec_ary;
    uint8_t f_cnt = 0;
    uint16_t array_max = ( scl_set.x_pixel_size ) * 2 - 1;

    if( scl_set.full_or_half == 2 && (*fnt_cnt) == 0) f_cnt = 1;

    for( i = 0; i < 16; i++ ){
      dec_ary = array_max - scl_set.x_scl_send_bytes;
      if( Fnt[ f_cnt ][ i ] & ( bt1 << scl_set.single_fnt_scl_cnt ) ){
        for( j = font.Xsize; j > 0; j-- ){
          scl_set.heap_array[ i ][ dec_ary-- ] = font.dot_lsb; //正スクロールと逆のバイト配列になり、LSBが先行
          scl_set.heap_array[ i ][ dec_ary-- ] = font.dot_msb;
          if( dec_ary < 0 ) break;
        }
      }else{
        for( j = font.Xsize; j > 0; j-- ){
          scl_set.heap_array[ i ][ dec_ary-- ] = font.bg_dot_lsb; //正スクロールと逆のバイト配列になり、LSBが先行
          scl_set.heap_array[ i ][ dec_ary-- ] = font.bg_dot_msb;
          if( dec_ary < 0 ) break;
        }
      }
    }

    scl_set.x_scl_send_bytes = scl_set.x_scl_send_bytes + font.Xsize * 2;
    if( scl_set.x_scl_send_bytes + 2 > array_max ) scl_set.x_scl_send_bytes = 0;

    uint16_t send_byte_max = array_max + 1;
    int16_t now_dec_send_byteNum = send_byte_max - scl_set.x_scl_send_bytes;

    if( now_dec_send_byteNum < 0 ){
      for( i = 0; i < 16; i++ ){
        for( j = font.Ysize; j > 0; j-- ){
          ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][ 0 ], send_byte_max );
        }
      }
    }else{
      for( i = 0; i < 16; i++ ){
        for( j = font.Ysize; j > 0; j-- ){
          ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][ now_dec_send_byteNum ], scl_set.x_scl_send_bytes );
          ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][ 0 ], now_dec_send_byteNum );
        }
      }
    }

    if( ++scl_set.single_fnt_scl_cnt > 7 ){
      scl_set.single_fnt_scl_cnt = 0;
      if( ++(*fnt_cnt) >= scl_set.font_sjis_len ) {
        *fnt_cnt = 0;
      }
    }

    scl_set.scl_last_time = millis();
    if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
  }
}
//********************************************************************
boolean ILI9341Spi::reverseScrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set ){
  boolean ret = false;
  scl_set.font_sjis_len = fontSJ_Length;

  ILI9341Spi::reverseScrolle8x16font( font, scl_set, &scl_set.full_or_half_cnt, fontSJ_Length, Fnt, xy_range_set );

  if( ( scl_set.full_or_half == 2 ) && ( scl_set.full_or_half_cnt == 2 ) ){
    scl_set.full_or_half_cnt = 0;
    ret = true;
  }else if( ( scl_set.full_or_half == 1 ) && ( scl_set.full_or_half_cnt == 1 ) ){
    scl_set.full_or_half_cnt = 0;
    ret = true;
  }
  return ret;
}
//********************************************************************
boolean ILI9341Spi::Yscrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set ){
  //Default xy_range_set = true
  boolean ret = false;
  if( millis() - scl_set.scl_last_time > scl_set.interval ){
    if( ( font.Xsize != font.prev_Xsize ) || ( font.Ysize != font.prev_Ysize ) ){
      ILI9341Spi::scrolleFormSet( font, scl_set,  scl_set.txt_width, scl_set.txt_height, font.Xsize, font.Ysize );
    }
    ILI9341Spi::spiSetChange();

    if( xy_range_set ){
      ILI9341Spi::rangeXY( font.x0, font.y0, font.x0 + scl_set.x_pixel_size - 1, font.y0 + scl_set.y_pixel_size - 1 );
      ILI9341Spi::commandWrite( 0x2C ); //RAM write
    }

    int i, j;
    uint16_t x_count = 0;
    int bit_count = 0;
    uint8_t bt1 = 0b10000000;
    uint16_t array_Y_max = scl_set.y_pixel_size / font.Ysize;

    for( i = 0; i < 2; i++ ){
      if( ( i == 1 ) && ( scl_set.full_or_half == 1 ) ){ //半角の場合、右半分はゼロで埋める
        for( bit_count = 0; bit_count < 8; bit_count++ ){
          for( j = font.Xsize; j > 0; j-- ){
            scl_set.heap_array[ scl_set.y_cnt ][ x_count++ ] = font.bg_dot_msb;
            scl_set.heap_array[ scl_set.y_cnt ][ x_count++ ] = font.bg_dot_lsb;
          }
        }
      }else{
        for( bit_count = 0; bit_count < 8; bit_count++ ){
          if( Fnt[ i ][ scl_set.single_fnt_scl_cnt ] & ( bt1 >> bit_count ) ){
            for( j = font.Xsize; j > 0; j-- ){
              scl_set.heap_array[ scl_set.y_cnt ][ x_count++ ] = font.dot_msb;
              scl_set.heap_array[ scl_set.y_cnt ][ x_count++ ] = font.dot_lsb;
            }
          }else{
            for( j = font.Xsize; j > 0; j-- ){
              scl_set.heap_array[ scl_set.y_cnt ][ x_count++ ] = font.bg_dot_msb;
              scl_set.heap_array[ scl_set.y_cnt ][ x_count++ ] = font.bg_dot_lsb;
            }
          }
        }
      }
    }

    scl_set.y_cnt++; 
    if( scl_set.y_cnt >= array_Y_max ) scl_set.y_cnt = 0;

    for( i = scl_set.y_cnt; i < array_Y_max; i++ ){
      for( j = 0; j < font.Ysize; j++){
        ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][ 0 ], scl_set.y_scl_send_bytes );
      }
    }
    for( i = 0; i < scl_set.y_cnt; i++ ){
      for( j = 0; j < font.Ysize; j++ ){
        ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][ 0 ], scl_set.y_scl_send_bytes );
      }
    }

    scl_set.single_fnt_scl_cnt++;
    if( scl_set.single_fnt_scl_cnt > 15 ){
      scl_set.single_fnt_scl_cnt = 0;
      ret = true;
    }

    scl_set.scl_last_time = millis();
    if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
  }
  return ret;
}
//********************************************************************
boolean ILI9341Spi::YdownScrolle8x16fontInc( FontParameter &font, ScrolleParameter &scl_set, const uint16_t &fontSJ_Length, uint8_t Fnt[][16], bool xy_range_set ){
  //Default xy_range_set = true
  boolean ret = false;
  if( ( millis() - scl_set.scl_last_time ) > scl_set.interval ){
    if( ( font.Xsize != font.prev_Xsize ) || ( font.Ysize != font.prev_Ysize ) ){
      ILI9341Spi::scrolleFormSet( font, scl_set,  scl_set.txt_width, scl_set.txt_height, font.Xsize, font.Ysize );
    }
    ILI9341Spi::spiSetChange();

    if( xy_range_set ){
      ILI9341Spi::rangeXY( font.x0, font.y0, font.x0 + scl_set.x_pixel_size - 1, font.y0 + scl_set.y_pixel_size - 1 );
      ILI9341Spi::commandWrite( 0x2C ); //RAM write
    }

    int i, j;
    uint8_t bt1 = 0b10000000;
    uint16_t x_count = 0;
    int bit_count = 0;
    uint16_t array_Y_max = scl_set.y_pixel_size / font.Ysize - 1;
    uint16_t scl_rev_Y_cnt = array_Y_max - scl_set.y_cnt;

    for( i = 0; i < 2; i++ ){
      if( ( i == 1 ) && ( scl_set.full_or_half == 1 ) ){ //半角の場合、右半分はゼロで埋める
        for( bit_count = 0; bit_count < 8; bit_count++ ){
          for( j = font.Xsize; j > 0; j-- ){
            scl_set.heap_array[ scl_rev_Y_cnt ][ x_count++ ] = font.bg_dot_msb;
            scl_set.heap_array[ scl_rev_Y_cnt ][ x_count++ ] = font.bg_dot_lsb;
          }
        }
      }else{
        for( bit_count = 0; bit_count < 8; bit_count++ ){
          if( Fnt[i][ 15 - scl_set.single_fnt_scl_cnt ] & ( bt1 >> bit_count ) ){
            for( j = font.Xsize; j > 0; j-- ){
              scl_set.heap_array[ scl_rev_Y_cnt ][ x_count++ ] = font.dot_msb;
              scl_set.heap_array[ scl_rev_Y_cnt ][ x_count++ ] = font.dot_lsb;
            }
          }else{
            for( j = font.Xsize; j > 0; j-- ){
              scl_set.heap_array[ scl_rev_Y_cnt ][ x_count++ ] = font.bg_dot_msb;
              scl_set.heap_array[ scl_rev_Y_cnt ][ x_count++ ] = font.bg_dot_lsb;
            }
          }
        }
      }
    }

    for( i = scl_rev_Y_cnt; i <= array_Y_max; i++ ){
      for( j = 0; j < font.Ysize; j++ ){
        ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][0], scl_set.y_scl_send_bytes );
      }
    }
    for( i = 0; i < scl_rev_Y_cnt; i++ ){
      for( j = 0; j < font.Ysize; j++ ){
        ILI9341Spi::dataWriteBytes( &scl_set.heap_array[ i ][0], scl_set.y_scl_send_bytes );
      }
    }

    scl_set.y_cnt++; 
    if( scl_set.y_cnt > array_Y_max ) scl_set.y_cnt = 0;

    scl_set.single_fnt_scl_cnt++;
    if( scl_set.single_fnt_scl_cnt > 15 ){
      scl_set.single_fnt_scl_cnt = 0;
      ret = true;
    }

    scl_set.scl_last_time = millis();
    if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
  }
  return ret;
}

//********************************************************************
void ILI9341Spi::display8x16Font( FontParameter &font, uint16_t fontSJ_Length, uint8_t Fnt[][16] ){
  ILI9341Spi::spiSetChange();

  font.txt_width = fontSJ_Length;
  ILI9341Spi::fontParamMaxClip( font );

  uint8_t bt = 0b00000001;
  uint8_t dot_MSB = ( font.red << 3 ) | ( font.green >> 3 );
  uint8_t dot_LSB = ( font.green << 5 ) | font.blue;

  uint8_t bg_dot_MSB = 0;
  uint8_t bg_dot_LSB = 0;
  if( ( font.bg_red + font.bg_green + font.bg_blue ) != 0 ){
    bg_dot_MSB = ( font.bg_red << 3 ) | ( font.bg_green >> 3 );
    bg_dot_LSB = ( font.bg_green << 5 ) | font.bg_blue;
  }

  uint16_t array_max = font.x_pixel_size * 2;
  uint8_t disp_byte[ array_max ] = {};
  uint16_t byte_cnt = 0;
  uint16_t X_pix_cnt = font.x0, Y_pix_cnt = font.y0;
  uint16_t Y_tmp_range = 0;

  int i, j, k, ii, jj;
  for( i = 0; i < 16; i++ ){
    for( j = 0; j < font.txt_width; j++ ){
      for( k = 7; k >= 0; k-- ){
        if( Fnt[ j ][ i ] & ( bt << k ) ){
          for( ii = font.Xsize; ii > 0; ii-- ){
            disp_byte[ byte_cnt++ ] = dot_MSB;
            disp_byte[ byte_cnt++ ] = dot_LSB;
            X_pix_cnt++;
            if( byte_cnt >= array_max ) goto EXIT_max_pixX;
          }
        }else{
          for( ii = font.Xsize; ii > 0; ii-- ){
            disp_byte[ byte_cnt++ ] = bg_dot_MSB;
            disp_byte[ byte_cnt++ ] = bg_dot_LSB;
            X_pix_cnt++;
            if( byte_cnt >= array_max ) goto EXIT_max_pixX;
          }
        }
      }
    }
    
EXIT_max_pixX:

    Y_tmp_range = Y_pix_cnt + font.Ysize -1;
    if( Y_tmp_range > ILI9341Spi::m_max_pix_y1 ) Y_tmp_range = ILI9341Spi::m_max_pix_y1;
    ILI9341Spi::rangeXY( font.x0, Y_pix_cnt, X_pix_cnt - 1, Y_tmp_range );
    ILI9341Spi::commandWrite( 0x2C ); //RAM write
    for( jj = 0; jj < font.Ysize; jj++ ){
      ILI9341Spi::dataWriteBytes( disp_byte, byte_cnt );
      Y_pix_cnt++;
      if( Y_pix_cnt > ILI9341Spi::m_max_pix_y1 ) goto EXIT;
    }
    X_pix_cnt = font.x0;
    byte_cnt = 0;
  }

EXIT:
  if( !mp_useHw_Cs ) digitalWrite( mp_cs, HIGH );
}


}  // namespace mynamespace
