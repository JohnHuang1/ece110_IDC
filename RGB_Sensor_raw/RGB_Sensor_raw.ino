#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */

  const int blue_light_pin= 44;
  const int red_light_pin = 45;
  const int green_light_pin = 46;
  
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

  int red = 0;
  int blue = 0;
  int green = 0;
  int ct = 0;

  bool ledOn = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  delay(500);
  while(1){
    if (tcs.begin()) {
      Serial.println("Found sensor");
      break;
    } else {
      Serial.println("No TCS34725 found ... check your connections");
    }
    delay(500);
  }
}


void loop() {
//  float r, g, b, c, colorTemp, lux;
  uint16_t r, g, b, c, colorTemp, lux;


  tcs.getRawData(&r, &g, &b, &c);
//  tcs.getRGB(&r, &g, &b);

  red = (int) r;
  green = (int) g;
  blue = (int) b;
  ct = (int) c;
  
  // colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  // lux = tcs.calculateLux(r, g, b);

  delay(100);
  Serial.print("R: "); Serial.print(red, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(green, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(blue, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print((int) c, DEC); Serial.print(" ");
  Serial.println(" ");

//  RGB_color(red, green, blue);
//  return;

  //green: r=88 g=96 b = 43
  //gray r=100 g=76 b=54
  //yellow r=114 g=82 b=32
  //magenta r=125 g=56 b=55
  //red r=169 g=42 b=37
  //blue r=79 g=73 b=80

  //GREEN
  // r=74 g=109 b = 45
  // r=820 g=1160 b=490 c=2760
  if(valInRange(red, 820) && 
  valInRange(green, 1160) &&
  valInRange(blue, 490))
  {
    Serial.println("GREEN detected");
    RGB_color(0, 255, 0);
    return;
  }

  //GRAY
  // r=81 g=84 b=67
  // r=1300 g=1300 b=1000 c=3210
  if(valInRange(red, 1300) && 
  valInRange(green, 1300) &&
  valInRange(blue, 1000))
  {
    Serial.println("GRAY detected");
    RGB_color(125, 125, 125);
    return;
  }

  //YELLOW
  // r=99 g=95 b=33
  // r=2100 g=1860 b=670 c=5180
  if(valInRange(red, 2100) && 
  valInRange(green, 1860) && 
  valInRange(blue, 670))
  {
    Serial.println("YELLOW detected");
    RGB_color(255, 255, 0);
    return;
  }
  //MAGENTA
  // r=105 g=57 b=76
  // r=900 g=460 b=570 c=2060
  if(valInRange(red, 900) && 
  valInRange(green, 460) &&
  valInRange(blue, 570))
  {
    Serial.println("MAGENTA detected");
    RGB_color(255, 0, 255);
    return;
  }

  //RED
  // r=168 g=42 b=35
  // r=1250 g=260 b=230 c=1660
  if(valInRange(red, 1250) && 
  valInRange(green, 300) &&
  valInRange(blue, 300))
  {
    Serial.println("RED detected");
    RGB_color(255, 0, 0);
    return;
  }
  
  //BLUE
  // r=58 g=74 b=103
  // r=370 g=450 b=600 c=1530
  if(valInRange(red, 370) && 
  valInRange(green, 450) &&
  valInRange(blue, 600))
  {
    Serial.println("BLUE detected");
    RGB_color(0, 0, 255);
    return;
  }

  //OTHER
  RGB_color(0, 0, 0);
  
}

bool valInRange(int color, int target){
  int threshold = 200;
  return color < target + threshold && color > target - threshold;
}

void RGB_color(int red_light, int green_light, int blue_light) {
  // 255 - colorVal => makes the RGB LED return the correct color b/c 0 = on | 255 = off
  analogWrite(blue_light_pin, 255 - blue_light);
  analogWrite(red_light_pin, 255 - red_light);
  analogWrite(green_light_pin, 255 - green_light);
}
