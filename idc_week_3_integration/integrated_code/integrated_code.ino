#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define Rx 17
#define Tx 16
#include <SoftwareSerial.h>

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

/* Initialise color sensor with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

bool finishedLineFollow = false; // Keep track of if Line follow is finished

int values[6] = {-1,-1,-1,-1,-1,-1}; // Challenge value array
int teamSum = -1; // Team sum integer stored

// index 0 = noMag | index 1 = mag present
int magReads[2] = {0,0};
int magChar = 'z'; // z  = default no read, 
int mIndex = -1;
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
String lcdTop = "";
String lcdBottom = "";
  
int valuesStored = 0;
String challenges[] = {"Manage the Nitrogen Cycle", 
                      "Advance Health Informatics", 
                      "Engineer Better Medicine",
                      "Enhance Virtual Reality",
                      "Engineer the Tools of Scientific Discovery",
                      "Reverse-Engineer the Brain",
                      "Advance Personalized Learning"};
long timestamp = 0;

void setup() {
//  tone(4, 3000, 3000);
//  delay(1000);

  // LCD DataPin
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);

  Serial.begin(9600); // Debug Serial
  Serial2.begin(9600); // xBee Serial
  mySerial.begin(9600); // LCD Screen

  servoLeft.attach(12); // Servo Left output
  servoRight.attach(11); // Servo Right output

  //LED Pins
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  // Hall-effect Pin
  pinMode(A2, INPUT);

  mySerial.write(12);
  mySerial.write(18);
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

void loop() {
  
  // Line Following loop, Escapes when all-black case is hit
  if(!finishedLineFollow){
    Serial.print("Line Following");
    mySerial.write(12);
    lcdTop = "Line Following";
    LCD();
    RGB_color(0,0,0);
    while(!followLine()){
      delay(1);
    }
    finishedLineFollow = true;
    Serial.println("Now Finished");
  }

  servoLeft.detach(); // Servo Left output
  servoRight.detach(); // Servo Right output


  if(valuesStored < 6){
    checkReceive(); // Reads data from xBee and stores value if appropriate
  }

  int attempts = 10; // Number of Sensor attempts to do
  if(sensorReadAttempts < attempts){

    lcdTop = "Attempt # " + String(sensorReadAttempts);
    LCD();

    magReads[doMagnetSense()] += 1; // Adds magnet reading to appropriate index of array

    int rgbIndex = doRGBStuff();
    if(rgbIndex > -1) { // Test for if RGB output valid
      RGBReads[rgbIndex] += 1; // Adds RGB reading to approriate index of array
    }
    
    sensorReadAttempts++; // Increment sensorReadAttemtps
    
    // Case for after last read attempt is reached
    if(sensorReadAttempts == attempts){

      delay(100);
      mySerial.write(12);
      mySerial.write(17);
      delay(100);
      
      Serial.print("magReads = ");
      printIntArray(2, magReads);
      Serial.print("RGBReads = ");
      printIntArray(6, RGBReads);
      
      mIndex = getMaxIndexOfArr(2, magReads); // Get highest value of magnet readings array
      // Assign correct char to magChar
      if(mIndex) {
        magChar = 'f';
      } else {
        magChar = 'e';
      }
      storeValue(magChar); // Store own task output to values Array

      continentIndex = getMaxIndexOfArr(6, RGBReads); // store RGB readings output by getting largest value of RGB readings array

      //TODO Display Continent name via continentIndex;
      lcdTop = continentNames[continentIndex] + "--" + mIndex;
      LCD();

      Serial.print("magChar = ");
      Serial.print((char) magChar);
      Serial.print(" | Continent = ");
      Serial.println(continentNames[continentIndex]);
//      teamSum = 6;
//      valuesStored = 6;
    }
    Serial.print("Sensor Read Attempt: ");
    Serial.println(sensorReadAttempts);
  } else {
    if(millis() - timestamp > 3000){
      sendMessage(magChar);
      timestamp = millis();
    }
//    sendMessage(magChar);
//    delay(100);
  }

  // Case for all challenge values stored
  if(valuesStored == 6) {
    Serial.println(challenges[getSum()]);
    lcdBottom = challenges[getSum()] + " | " + getSum() + " | " + lcdBottom;
    Serial.print("loop");
    valuesStored++; // increment values stored so LCD isn't constantly updated
  } else if(valuesStored > 6) {
    lcdTop = continentNames[continentIndex] + "--" + mIndex;
    LCD();
  }
//  Serial.print("loop");
  delay(100);
  
}

// move Forward funciton
void goForward(int time){
  servoLeft.writeMicroseconds(1550);
  servoRight.writeMicroseconds(1450);
  delay(time);
}

// move Backward funciton
void goBackward(int time){
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
  delay(time);
}

// Stop moving funciton
void stopMoving(int time){
  // lastMove = 4;
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(time);
}

// rotate Left funciton
void goLeft(int time){
  // lastMove = 1;
  servoLeft.writeMicroseconds(1475);
  servoRight.writeMicroseconds(1475);
  delay(time);
}

// rotate Right funciton
void goRight(int time){
  // lastMove = 2;
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1525);
  delay(time);
}

// Read data from QTI sensors
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
   return duration; // Duration is the time it takes for capacitor of QTI sensor to discharge to a certain voltage
}

// Send message to xBee
void sendMessage(char c){
  Serial.print("Sending: ");
  Serial.println(c);
  Serial2.print(c);
}

// Read message from xBee
bool checkReceive(){
  if(Serial2.available()){
    char c = Serial2.read();
    Serial.print("Receiving: ");
    Serial.println(c + String(int(c)));
    storeValue(c); // Store value from xBee
    return true;
  } else {
    return false;
  }
}

//Store value to values Array
void storeValue(char c){
  Serial.println("StoreValue Called");
  if(valuesStored < 6 && c >= 97 && c <= 108){ // Check if input is valid
    if(values[(c - 97) / 2] == -1) {
      values[(c - 97) / 2] = (c + 1) % 2;
      valuesStored++;

      Serial.println("lcdBottom +=" + String(c));
      lcdBottom += String(c) + " ";
      LCD();
      
      printIntArray(6, values);
      Serial.print("sum = ");
      Serial.println(getSum());
    }
  }
}

// Print array values to Serial (DEBUG)
void printIntArray(int arrLength, int arr[]){
  Serial.print("[");
  for(int i = 0; i < arrLength; i++){
    Serial.print((int) arr[i]);
    Serial.print(",");
  }
  Serial.println("]");
}

void printFloatArray(int arrLength, float arr[]){
  Serial.print("[");
  for(int i = 0; i < arrLength; i++){
    Serial.print((float) arr[i]);
    Serial.print(",");
  }
  Serial.println("]");
}


// fetch sum of challenge values stored
int getSum(){
  if(teamSum == -1 && valuesStored >= 6){
    int val = 0;
    for(int i = 0; i < 6; i++){
      val += values[i];
    }
    teamSum = val;
    return val;
  }
  return teamSum;
}

// Get reading of magnet. Output to true or false
bool doMagnetSense(){
  int val = analogRead(A2);
  int threshold = 30;
  int noMag = 522;
  Serial.println(val);
  if(val < noMag - threshold || val > noMag + threshold){
    Serial.println("Magnet Detected");
    return true;
  } else {
    Serial.println("Magnet NOT Detected");
    return false;
  }
}

// Follow Line code
bool followLine(){
  // true = on black line ~200 ish
  // false = not on black line ~30 ish
  int threshold = 80;
  
  // Get values from QTI Sensors
  long middleVal = RCTime(QTIMiddle);
  bool middle = RCTime(QTIMiddle) > threshold;
  long leftVal = RCTime(QTILeft);
  bool left = RCTime(QTILeft) > threshold;
  long rightVal = RCTime(QTIRight);
  bool right = RCTime(QTIRight) > threshold;

//  Serial.print(" middle : ");
//  Serial.println(middleVal);
//  Serial.print(" left :  ");
//  Serial.println(leftVal);
//  Serial.print(" right :  ");
//  Serial.println(rightVal);
//   delay(500);
  int time = 1; // Move time

  // All-black / Hash case
  if(middle && left && right) {
    Serial.println("HASH REACHED");
    stopMoving(1000);
    return true;
  }

  // Line centered
  if(middle && !left && !right){
    Serial.println("FORWARD REACHED");
    goForward(time);
    return false;
  }

  // On left of line
  if(!left && right){
    Serial.println("GORIGHT REACHED");
    goRight(time);
    return false;
  }

  // On Right of Line
  if(left && !right){
    Serial.println("GOLEFT REACHED");
    goLeft(time);
    return false;
  }

  // no case reached
  Serial.println("NOCASE REACHED");
  return false;
}

// Get maximum value of array
int getMaxIndexOfArr(int arrLength, int arr[]){
  int maxIndex = 0;
  for(int i = 0; i < arrLength; i++){
    if(arr[i] > arr[maxIndex]){
      maxIndex = i;
    }
  }
  return maxIndex;
}

// Fetch RGB Reading convert to corresponding integer index for RGB Readings array
int doRGBStuff(){
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  int red = (int) r;
  int green = (int) g;
  int blue = (int) b;
  

  // Scale RGB values to ratios of the highest RGB value
  int rgbArr[3] = {red, green, blue};
  int greatestIndex = getGreatest(rgbArr);
  float ratios[3] = {};
  getRatios(rgbArr, greatestIndex, ratios);
//  float ratios[3] = {0.81, 1.0, 0.51};
  printFloatArray(3, ratios);  

  //gray
  float grayTarget[3] = {1.0, 0.99, 0.78};
  if(compareRatios(ratios, grayTarget)){
    Serial.println("GRAY detected");
    RGB_color(125, 125, 125);
    return 0;
  }

    //green
  float greenTarget[3] = {0.71, 1.0, 0.41};
  if(compareRatios(ratios, greenTarget)){
    Serial.println("GREEN detected");
    RGB_color(0, 255, 0);
    return 1;
  }

  //yellow
  float yellowTarget[3] = {1.0, 0.89, 0.32};
  if(compareRatios(ratios, yellowTarget)){
    Serial.println("YELLOW detected");
    RGB_color(255, 255, 0);
    return 2;
  }
  
  //blue
  float blueTarget[3] = {0.65, 0.77, 1.0};
  if(compareRatios(ratios, blueTarget)){
    Serial.println("BLUE detected");
    RGB_color(0, 0, 255);
    return 3;
  }

  
  //red
  float redTarget[3] = {1.0, 0.24, 0.20};
  if(compareRatios(ratios, redTarget)){
    Serial.println("RED detected");
    RGB_color(255, 0, 0);
    return 4;
  }

  //magenta
  float magentaTarget[3] = {1.0, 0.51, 0.63};
  if(compareRatios(ratios, magentaTarget)){
    Serial.println("MAGENTA detected");
    RGB_color(255, 0, 255);
    return 5;
  }

  //OTHER
  RGB_color(0, 0, 0);
  return -1;
}

// get greatest of the 3 RGB inputs
int getGreatest(int input[3]){
  int gIndex = 0;
  
//  if(input[0] >= input[1] && input[0] >= input[2]) indexOfG = 0;
//  if(input[1] >= input[2] && input[1] >= input[0]) indexOfG = 1;
//  if(input[2] > input[1] && input[2] > input[0]) indexOfG = 2;
  
  for(int i = 1; i < 3; i++){
    if(input[i] > input[gIndex]){
      gIndex = i;
    }
  }
  return gIndex;
}


// Set values of ratio float array from RGB integer array
void getRatios(int input[3], int greatestIndex, float ratioArr[3]){
  ratioArr[greatestIndex] = 1.0;
  for(int i = 0; i < 3; i++){
    ratioArr[i] = (float) input[i] / (float) input[greatestIndex];
  }
}

// Check if data float values are in threshold of target values
boolean compareRatios(float data[3], float target[3]){
  float threshold = 0.2;
  Serial.println("Compare ratios");
  for(int i = 0; i < 3; i++){
//    Serial.print(String(i) + " | data = " + String(data[i]) + " |  lowerBound = " + String(target[i] - threshold) + " | upperBound = " + String(target[i] + threshold));
    boolean val = data[i] < target[i] - threshold || data[i] > target[i] + threshold;
//    Serial.println( " | val = " + String(val));
    if(val) return false;
  }
  return true;
}

// Set RGB LED 
void RGB_color(int red_light, int green_light, int blue_light) {
  // 255 - colorVal => makes the RGB LED return the correct color b/c 0 = on | 255 = off
  analogWrite(blue_light_pin, 255 - blue_light);
  analogWrite(red_light_pin, 255 - red_light);
  analogWrite(green_light_pin, 255 - green_light);
}

// Set LCD to write 2 strings on the top and bottom
void LCD() {
  Serial.println("LCD Called");
  mySerial.write(12);   
  delay(5);
  mySerial.write(128); //move to pos 0,0
  mySerial.print(lcdTop);
  mySerial.write(148); // move to pos 1,0
  mySerial.print(getLCDBottomString());
  if(pos == 0 || pos > lcdBottom.length() - 13){
    delay(500);
  }
  delay(100);
}
//Helper method to scroll bottom line
String getLCDBottomString(){
  if(lcdBottom.length() < 16) return lcdBottom;
  pos ++;
  if(pos > lcdBottom.length() - 14) {
    pos = 0;
  }
  int endpos = pos + 16;
  if(endpos > lcdBottom.length()){
    endpos = lcdBottom.length();
  }
  return lcdBottom.substring(pos, endpos);
}
