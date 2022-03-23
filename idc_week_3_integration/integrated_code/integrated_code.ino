//# include <Servo_Hardware_PWM.h>
# include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define hall_sensor 2
#define Rx 17
#define Tx 16

const int LED_PIN = 8;
const int BUTTON_PIN = 2;

int threshold = 20;
int noMag = 522;
const int HALL_OUTPUT_PIN = 10;

Servo servoLeft; // higher number = counter clockwise
Servo servoRight; // lower number = clockwise

int QTIMiddle = 51;
int QTILeft = 49;
int QTIRight = 53;

/*
  -1 : none
  0 : hash
  1 : on left of line
  2 : on right of line
  3 : off line
  4 : on line
*/
int lastSense = -1;

/* Initialise with default values (int time = 2.4ms, gain = 1x) */

  const int blue_light_pin= 44;
  const int red_light_pin = 45;
  const int green_light_pin = 46;
  
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

  int red = 0;
  int blue = 0;
  int green = 0;
  int ct = 0;

  bool finishedLineFollow = false;

void setup() {
  tone(4, 3000, 3000);
  delay(1000);

  Serial.begin(9600);
  Serial2.begin(9600);

  servoLeft.attach(12);
  servoRight.attach(11);

  pinMode(LED_PIN, OUTPUT);

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  pinMode(A2, INPUT);
  pinMode(HALL_OUTPUT_PIN, OUTPUT);

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

  if(!finishedLineFollow){
    while(!followLine()){
      delay(1);
    }
    finishedLineFollow = true;
  }

  doRGBStuff();
  checkReceive();
  if(doMagnetSense()){
    sendMessage();
  }
  delay(10);
  
}

void goForward(int time){
  // lastMove = 0;
  servoLeft.writeMicroseconds(1550);
  servoRight.writeMicroseconds(1450);
  delay(time);
}

void goBackward(int time){
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
  delay(time);
}

void goBackwardRight(int time){
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1600);
  delay(time);
}


void stopMoving(int time){
  // lastMove = 4;
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(time);
}

void goLeft(int time){
  // lastMove = 1;
  servoLeft.writeMicroseconds(1475);
  servoRight.writeMicroseconds(1475);
  delay(time);
}

void goForwardLeft(int time){
  servoLeft.writeMicroseconds(1510);
  servoRight.writeMicroseconds(1475);
  delay(time);
}

void goRight(int time){
  // lastMove = 2;
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
  delay(time);
}

long RCTime(int sensorIn){
   long duration = 0;
   pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
   digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
   delay(1);                      // Wait 1ms
   pinMode(sensorIn, INPUT);      // Make pin INPUT
   digitalWrite(sensorIn, LOW);   // Turn off internal pullups
   while(digitalRead(sensorIn)){  // Wait for pin to go LOW
      duration++;
   }
   return duration;
}

void sendMessage(){
  char outgoing = '4';
  Serial.print("Sending: ");
  Serial.println(outgoing);
  Serial2.print(outgoing);
}

bool checkReceive(){
  if(Serial2.available()){
    char incoming = Serial2.read();
    Serial.print("Receiving: ");
    Serial.println(incoming);
    digitalWrite(LED_PIN, HIGH);
    return true;
  } else {
    digitalWrite(LED_PIN, LOW);
    return false;
  }
}

bool doMagnetSense(){
  int val = analogRead(A2);
//  Serial.println(val);
  if(val < noMag - threshold || val > noMag + threshold){
    digitalWrite(HALL_OUTPUT_PIN, HIGH);
    Serial.println("Magnet Detected");
    return true;
  } else {
    digitalWrite(HALL_OUTPUT_PIN, LOW);
    Serial.println("Magnet NOT Detected");
    return false;
  }
}

bool followLine(){
  
  // true = on black line ~200 ish
  // false = not on black line ~30 ish
  int threshold = 80;
  long middleVal = RCTime(QTIMiddle);
  bool middle = RCTime(QTIMiddle) > threshold;
  long leftVal = RCTime(QTILeft);
  bool left = RCTime(QTILeft) > threshold;
  long rightVal = RCTime(QTIRight);
  bool right = RCTime(QTIRight) > threshold;

  Serial.print(" middle : ");
  Serial.println(middleVal);
  Serial.print(" left :  ");
  Serial.println(leftVal);
  Serial.print(" right :  ");
  Serial.println(rightVal);
  // delay(500);
  int time = 1;
  
  if(middle && left && right) {
    // on hash
//    if(lastSense == 0){
//      goForward(25);
//      return;
//    }
//    lastSense = 0;
    stopMoving(1000);
    // goForward(50);
//    goLeft(50);
    
    return true;
  }


  if(middle && !left && !right){
    //on line
    lastSense = 4;
    goForward(time);
    return false;
  }

  if(!left && right){
    //on left of line
    lastSense = 1;
    goRight(time);
    return false;
  }

  if(left && !right){
    // on right of line
    lastSense = 2;
    goLeft(time);
    return false;
  }

  return false;
}

void doRGBStuff(){
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);

  red = (int) r;
  green = (int) g;
  blue = (int) b;

  int rgbArr[3] = {red, green, blue};
  int greatestIndex = getGreatest(rgbArr);
  float ratios[3] = {};
  getRatios(rgbArr, greatestIndex, ratios);

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

void RGB_color(int red_light, int green_light, int blue_light) {
  // 255 - colorVal => makes the RGB LED return the correct color b/c 0 = on | 255 = off
  analogWrite(blue_light_pin, 255 - blue_light);
  analogWrite(red_light_pin, 255 - red_light);
  analogWrite(green_light_pin, 255 - green_light);
}
