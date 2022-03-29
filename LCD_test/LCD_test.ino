#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
//#define hall_sensor 2
#define Rx 17
#define Tx 16
const int TxPin = 6;
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

Servo servoLeft; // higher number = counter clockwise
Servo servoRight; // lower number = clockwise

// QTI Pins
int QTIMiddle = 51;
int QTILeft = 49;
int QTIRight = 53;

// RGB LED Pins
const int blue_light_pin= 44;
const int red_light_pin = 45;
const int green_light_pin = 46;

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

bool finishedLineFollow = false; // Keep track of if Line follow is finished

int values[6] = {-1,-1,-1,-1,-1,-1}; // Challenge value array
int teamSum = -1; // Team sum integer stored

// index 0 = noMag | index 1 = mag present
int magReads[2] = {0,0};
int magChar = 'z'; // z  = default no read, 
int sensorReadAttempts = 0;

// index corresponds to continent in continentNames array
int RGBReads[6] = {0,0,0,0,0,0};
String continentNames[6] = {
  "South America", // Gray
  "North America", // Green
  "Africa", // Yellow
  "Europe", // Blue
  "Asia", // Red
  "Australia" //Purple
  };
int continentIndex = -1;

//Scroll bottom line
int pos = -1;
String lcdBottom = "";
  
int valuesStored = 0;
String challenges[] = {"Manage the Nitrogen Cycle", 
                      "Advance Health Informatics", 
                      "Engineer Better Medicine",
                      "Enhance Virtual Reality",
                      "Engineer the Tools of Scientific Discovery",
                      "Reverse-Engineer the Brain",
                      "Advance Personalized Learning"};

void setup() {


  // LCD DataPin
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);

  Serial.begin(9600); // Debug Serial
  Serial2.begin(9600); // xBee Serial
  mySerial.begin(9600); // LCD Screen

//  servoLeft.attach(12); // Servo Left output
//  servoRight.attach(11); // Servo Right output

  //LED Pins
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  // Hall-effect Pin
  pinMode(A2, INPUT);

  delay(100);
  mySerial.write(12);
  mySerial.write(17);
  delay(100);

  // Init Color Sensor
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
void loop(){

//  int top = 0;
//  int bottom = 1;
//  for(int i = 0; i < 256){
//    top += 2;
//    bottom++;
//    LCD(""+ top, "" + bottom);
//  }
  if(lcdBottom.equals("")){
    lcdBottom = "Engineer the Tools of Scientific Discovery";
  }
  LCD("North America--e");
  Serial.print("loop");


}

 //Str 1 ~ Continent
 //Str 2 ~ Grand Challenge
void LCD(String topString) {
  mySerial.write(12);   
  delay(5);
  mySerial.write(128); //move to pos 0,0
  mySerial.print(topString);
  mySerial.write(148); // move to pos 1,0
  mySerial.print(getLCDBottomString());
//  mySerial.write(212);
//  mySerial.write(220);
  delay(500);
}

String getLCDBottomString(){
  if(lcdBottom.length() < 16) return lcdBottom;
  pos ++;
  if(pos > lcdBottom.length() - 1) {
    pos = 0;
  }
  int endpos = pos + 16;
  if(endpos > lcdBottom.length()){
    endpos = lcdBottom.length();
  }
  return lcdBottom.substring(pos, endpos);
}
