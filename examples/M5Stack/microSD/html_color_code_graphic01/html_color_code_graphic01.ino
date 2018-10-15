#define MGO_TEC_BV1_M5STACK_SD_SKETCH
#include <mgo_tec_bv1_m5stack_sd_simple1.h>

void setup() {
  LCD.ILI9341init();
  LCD.brightness( 255 ); //LCD LED Full brightness
}

void loop() {
  uint16_t x0 = 32, y0 = 64, x1 = 272, y1 = 112;
  String html_color_code = "#FFFFFF";
  x0 = 0, y0 = 0;
  LCD.drawPixel65k3Color_sd( x0, y0, html_color_code );
  x0 = 319, y0 = 0;
  LCD.drawPixel65k3Color_sd( x0, y0, html_color_code );
  x0 = 0, y0 = 239;
  LCD.drawPixel65k3Color_sd( x0, y0, html_color_code );
  x0 = 319, y0 = 239;
  LCD.drawPixel65k3Color_sd( x0, y0, html_color_code );
  delay(2000);

  x0 = 0, x1 = 319, y0 = 0;
  LCD.drawHorizontalLine( x0, x1, y0, html_color_code );
  delay(500);
  x0 = 319, y0 = 0, y1 = 239;
  LCD.drawVerticallLine( x0, y0, y1, html_color_code );
  delay(500);
  x0 = 319, x1 = 0, y0 = 239;
  LCD.drawHorizontalLine( x0, x1, y0, html_color_code );
  delay(500);
  x0 = 0, y0 = 239, y1 = 0;
  LCD.drawVerticallLine( x0, y0, y1, html_color_code );
  delay(500);
  x0 = 0, y0 = 0, x1 = 319, y1 = 239;
  LCD.drawLine( x0, y0, x1, y1, "#FF0000" );
  x0 = 319, y0 = 0, x1 = 0, y1 = 239;
  LCD.drawLine( x0, y0, x1, y1, "#00FF00");
  x0 = 160, y0 = 0, x1 = 160, y1 = 239;
  LCD.drawLine( x0, y0, x1, y1, "#0000FF" );
  x0 = 0, y0 = 120, x1 = 319, y1 = 120;
  LCD.drawLine(0, 120, 319, 120, "#FFFF00");
  delay(2000);

  x0 = 0, y0 = 0, x1 = 319, y1 = 239;
  LCD.drawRectangleFill( x0, y0, x1, y1, "white" );
  delay(1000);
  LCD.drawRectangleFill( x0, y0, x1, y1, "red" );
  delay(1000);
  LCD.drawRectangleFill( x0, y0, x1, y1, "green" );
  delay(1000);
  LCD.drawRectangleFill( x0, y0, x1, y1, "blue" );
  delay(1000);
  LCD.drawRectangleFill( x0, y0, x1, y1, "yellow" );
  delay(1000);
  LCD.drawRectangleFill( x0, y0, x1, y1, "cyan" );
  delay(1000);
  LCD.drawRectangleFill( x0, y0, x1, y1, "magenta" );
  delay(1000);
  LCD.drawRectangleFill( x0, y0, x1, y1, "black" );
  delay(2000);

  LCD.displayClear();
  int i, j;
  for( i = 0; i < 120 ; i = i + 5 ){
    LCD.drawRectangleLine( 40 + i, i, 279 - i, 239 - i, "white" );
  }
  delay(2000);

  uint8_t Width = 50;
  LCD.drawRectangleFill( 0, 0, 319, 239, "#0000ff" );
  LCD.drawRectangleFill( 10, 10, 10 + Width, 10 + Width, "#ff2211" );
  LCD.drawRectangleFill( 309 - Width, 10, 309, 10 + Width, "#5Affcc" );
  LCD.drawRectangleFill( 10, 229 - Width, 10 + Width, 229, "#77FF66" );
  LCD.drawRectangleFill( 309 - Width, 229 - Width, 309, 229, "#baff33" );
  delay(2000);

  LCD.displayClear();
  x0 = 40, y0 = 0, x1 = 279, y1 = 239;
  LCD.drawRectangleLine( x0, y0, x1, y1, "#0000ff" );
  x0 = 40, y0 = 0, x1 = 279, y1 = 239;
  LCD.drawLine( x0, y0, x1, y1, "#0000ff" );
  x0 = 40, y0 = 239, x1 = 279, y1 = 0;
  LCD.drawLine( x0, y0, x1, y1, "#0000ff" );
  x0 = 159, y0 = 119;
  uint16_t r = 119;
  LCD.drawCircleLine( x0, y0, r, "white" );
  x0 = 25, y0 = 25, r = 25;
  LCD.drawCircleLine( x0, y0, r, "#ff0000" );
  x0 = 294, y0 = 25, r = 25;
  LCD.drawCircleLine( x0, y0, r, "#00ff00" );
  x0 = 25, y0 = 214, r = 25;
  LCD.drawCircleLine( x0, y0, r, "#ffff00" );
  x0 = 294, y0 = 214, r = 25;
  LCD.drawCircleLine( x0, y0, r, "#ff00ff" );
  for( i = 0; i < 120; i = i + 5 ){
    LCD.drawCircleLine( 159, 119, 119-i, "white" );
  }
  delay(2000);

  LCD.displayClear();
  x0 = 0, y0 = 0, x1 = 319, y1 = 239;
  LCD.drawRectangleFill( x0, y0, x1, y1, "#000020" );
  x0 = 159, y0 = 119, r = 119;
  LCD.drawCircleFill( x0, y0, r, "#ff00ff" );
  x0 = 25, y0 = 25, r = 25;
  LCD.drawCircleFill( x0, y0, r, "#ff0000" );
  x0 = 294, y0 = 25, r = 25;
  LCD.drawCircleFill( x0, y0, r, "#00ff00" );
  x0 = 25, y0 = 214, r = 25;
  LCD.drawCircleFill( x0, y0, r, "#ffff00" );
  x0 = 294, y0 = 214, r = 25;
  LCD.drawCircleFill( x0, y0, r, "#00ffff" );
  x0 = 159, y0 = 119, r = 51;
  LCD.drawCircleFill( x0, y0, r, "#0000ff" );
  delay(2000);

  LCD.displayClear();
  for( i = 0; i < 240; i = i + 10 ){
    LCD.drawLine( 0, i, i, 239, "#10FF20" );
    LCD.drawLine( 80 + i, 0, 319, i, "#00ffff" );
  }
  delay(2000);

  LCD.displayClear();
  uint8_t red = 0, green = 0, blue = 0;
  //red (0-31), green (0-63), blue (0-31)
  j = 31;
  for( i = 0; i < 32; i++ ){
    red = i, green = 0, blue = 0;
    LCD.drawRectangleFill( i * 10, 0, i * 10 + 9, 59, red, green, blue );
    red = 0, green = i * 2, blue = 0;
    LCD.drawRectangleFill( i * 10, 60, i * 10 + 9, 119, red, green, blue );
    red = 0, green = 0, blue = i;
    LCD.drawRectangleFill( i * 10, 120, i * 10 + 9, 179, red, green, blue );
    red = j, green = j * 2, blue = j;
    LCD.drawRectangleFill( i * 10, 180, i * 10 + 9, 239, red, green, blue );
    j--;
  }
  delay(2000);
  LCD.displayClear(0, 0, 319, 239);
}
