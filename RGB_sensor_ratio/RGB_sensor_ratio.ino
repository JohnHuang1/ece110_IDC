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

  delay(100);
  Serial.print("R: "); Serial.print(red, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(green, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(blue, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print((int) c, DEC); Serial.print(" ");
  Serial.println(" ");
//
//  if(red > 2000 || green > 2000 || blue > 2000){
//    Serial.println("OTHER detected");
//    RGB_color(0, 0, 0);
//    return;
//  }

  int rgbArr[3] = {red, green, blue};
  int greatestIndex = getGreatest(rgbArr);
  float ratios[3] = {};
  getRatios(rgbArr, greatestIndex, ratios);
  printRatios(ratios);

  //gray = 1.0000000000, 0.9878141403, 0.7783701419, 
  //yellow = 1.0000000000, 0.8865698814, 0.3157894611, 
  //magenta = 1.0000000000, 0.5061590194, 0.6315789222,
  // red = 1.0000000000, 0.2409638643, 0.2022375106, 
  // blue = 0.6489184379, 0.7653909683, 1.0000000000,  
  // green = 0.7093023300, 1.0000000000, 0.4111295700,

  //gray
  float grayTarget[3] = {1.0, 0.99, 0.78};
  if(compareRatios(ratios, grayTarget)){
    Serial.println("GRAY detected");
    RGB_color(125, 125, 125);
    return;
  }

  //yellow
  float yellowTarget[3] = {1.0, 0.89, 0.32};
  if(compareRatios(ratios, yellowTarget)){
    Serial.println("YELLOW detected");
    RGB_color(255, 255, 0);
    return;
  }

  //magenta
  float magentaTarget[3] = {1.0, 0.51, 0.63};
  if(compareRatios(ratios, magentaTarget)){
    Serial.println("MAGENTA detected");
    RGB_color(255, 0, 255);
    return;
  }

  //red
  float redTarget[3] = {1.0, 0.24, 0.20};
  if(compareRatios(ratios, redTarget)){
    Serial.println("RED detected");
    RGB_color(255, 0, 0);
    return;
  }
  
  //blue
  float blueTarget[3] = {0.65, 0.77, 1.0};
  if(compareRatios(ratios, blueTarget)){
    Serial.println("BLUE detected");
    RGB_color(0, 0, 255);
    return;
  }

  //green
  float greenTarget[3] = {0.71, 1.0, .41};
  if(compareRatios(ratios, greenTarget)){
    Serial.println("GREEN detected");
    RGB_color(0, 255, 0);
    return;
  }

  //OTHER
  RGB_color(0, 0, 0);
  
}

bool valInRange(int color, int target){
  int threshold = 200;
  return color < target + threshold && color > target - threshold;
}

int getGreatest(int input[3]){
  int indexOfG = -1;
  if(input[0] >= input[1] && input[0] >= input[2]) indexOfG = 0;
  if(input[1] >= input[2] && input[1] >= input[0]) indexOfG = 1;
  if(input[2] > input[1] && input[2] > input[0]) indexOfG = 2;
  return indexOfG;
}

void getRatios(int input[3], int greatestIndex, float ratioArr[3]){
  ratioArr[greatestIndex] = 1.0;
  for(int i = 0; i < 3; i++){
    ratioArr[i] = (float) input[i] / (float) input[greatestIndex];
  }
}

boolean compareRatios(float data[3], float target[3]){
  float threshold = 0.2;
  for(int i = 0; i < 3; i++){
//    if(data[i] == 1.0 && data[i] != target[i]) return false;
    if(!(data[i] > target[i] - threshold && data[i] < target[i] + threshold)) return false;
  }
  return true;
}

void printRatios(float arr[3]){
  for(int i = 0; i < 3; i++){
    Serial.print(arr[i], DEC); Serial.print(", ");
  }
  Serial.println(" ");
}

void RGB_color(int red_light, int green_light, int blue_light) {
  // 255 - colorVal => makes the RGB LED return the correct color b/c 0 = on | 255 = off
  analogWrite(blue_light_pin, 255 - blue_light);
  analogWrite(red_light_pin, 255 - red_light);
  analogWrite(green_light_pin, 255 - green_light);
}
