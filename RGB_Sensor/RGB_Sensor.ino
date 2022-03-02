#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */

  const int blue_light_pin= 45;
  const int red_light_pin = 46;
  const int green_light_pin = 44;
  
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

  int red = 0;
  int blue = 0;
  int green = 0;

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
  float r, g, b, c, colorTemp, lux;

//  tcs.getRawData(&r, &g, &b, &c);
  tcs.getRGB(&r, &g, &b);

  red = (int) r;
  green = (int) g;
  blue = (int) b;
  
  // colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  // lux = tcs.calculateLux(r, g, b);

  delay(100);
  Serial.print("R: "); Serial.print(red, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(green, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(blue, DEC); Serial.print(" ");
  Serial.println(" ");

  //green: r=88 g=96 b = 43
  //gray r=100 g=76 b=54
  //yellow r=114 g=82 b=32
  //magenta r=125 g=56 b=55
  //red r=165 g=42 b=35
  //blue r=79 g=73 b=80

  //GREEN
  // r=88 g=96 b = 43
  if(valInRange(red, 88) && 
  valInRange(green, 96) &&
  valInRange(blue, 43))
  {
    Serial.println("GREEN detected");
    RGB_color(0, 255, 0);
    return;
  }

  //GRAY
  // r=100 g=76 b=54
  if(valInRange(red, 100) && 
  valInRange(green, 76) &&
  valInRange(blue, 54))
  {
    Serial.println("GRAY detected");
    RGB_color(125, 125, 125);
    return;
  }

  //YELLOW
  // r=114 g=82 b=32
  if(valInRange(red, 114) && 
  valInRange(green, 82) &&
  valInRange(blue, 32))
  {
    Serial.println("YELLOW detected");
    RGB_color(255, 255, 0);
    return;
  }
  //MAGENTA
  // r=125 g=56 b=55
  if(valInRange(red, 125) && 
  valInRange(green, 56) &&
  valInRange(blue, 55))
  {
    Serial.println("MAGENTA detected");
    RGB_color(255, 0, 255);
    return;
  }

  //RED
  // r=168 g=42 b=35
  if(valInRange(red, 168) && 
  valInRange(green, 42) &&
  valInRange(blue, 35))
  {
    Serial.println("RED detected");
    RGB_color(255, 0, 0);
    return;
  }
  
  //BLUE
  // r=79 g=73 b=80
  if(valInRange(red, 79) && 
  valInRange(green, 73) &&
  valInRange(blue, 80))
  {
    Serial.println("BLUE detected");
    RGB_color(0, 0, 255);
    return;
  }
  
}

bool valInRange(int color, int target){
  int threshold = 5;
  return color < target + 5 && color > target - 5;
}

void RGB_color(int red_light, int green_light, int blue_light) {
  // 255 - colorVal => makes the RGB LED return the correct color b/c 0 = on | 255 = off
  analogWrite(blue_light_pin, 255 - blue_light);
  analogWrite(red_light_pin, 255 - red_light);
  analogWrite(green_light_pin, 255 - green_light);
}
