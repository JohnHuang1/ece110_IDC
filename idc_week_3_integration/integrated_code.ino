#define hall_sensor 2
#include <Wire.h>
#include "Adafruit_TCS34725.h"
# include <Servo.h>

Servo servoLeft; // higher number = counter clockwise
Servo servoRight; // lower number = clockwise

int QTIMiddle = 51;
int QTILeft = 49;
int QTIRight = 53;


int lastSense = -1;

int threshold = 20;
int noMag = 522;
const int HALL_OUTPUT_PIN = 10;
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
  tone(4, 3000, 3000);
  delay(1000);

  Serial.begin(9600);

  servoLeft.attach(12);
  servoRight.attach(11);
  Serial.begin(9600);
  pinMode(A2, INPUT);
  pinMode(HALL_OUTPUT_PIN, OUTPUT);
}

void loop() {
 int val = analogRead(A2);
  Serial.println(val);
  if(val < noMag - threshold || val > noMag + threshold){
    digitalWrite(HALL_OUTPUT_PIN, HIGH);
  } else {
    digitalWrite(HALL_OUTPUT_PIN, LOW);
  }
  delay(100);
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

  int threshold = 80;
  long middleVal = RCTime(QTIMiddle);
  bool middle = RCTime(QTIMiddle) > threshold;
  long leftVal = RCTime(QTILeft);
  bool left = RCTime(QTILeft) > threshold;
  long rightVal = RCTime(QTIRight);
  bool right = RCTime(QTIRight) > threshold;
  int time = 1;
  
  if(middle && left && right) {
    // on hash
    if(lastSense == 0){
      goForward(25);
      return;
    }
    lastSense = 0;
    stopMoving(1000);
    // goForward(50);
    goLeft(50);
    return;
  }
  if(middle && !left && !right){
    //on line
    lastSense = 4;
    goForward(time);
    return;
  }

  if(!left && right){
    //on left of line
    lastSense = 1;
    goRight(time);
    return;
  }

  if(left && !right){
    // on right of line
    lastSense = 2;
    goLeft(time);
    return;
  }

}

void goForward(int time){
  // lastMove = 0;
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
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

