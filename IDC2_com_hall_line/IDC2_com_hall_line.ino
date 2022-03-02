/*
    IDC 2 Line Following, Information Gathering & Communication
    Contains:
      ICD Circle line following
      Working hall-effect sensor with indicator LED
      Communication using Xbee (push button to send, led indicator when received)
*/
# include <Servo.h>
#define Rx 17
#define Tx 16

Servo servoLeft; // higher number = counter clockwise
Servo servoRight; // lower number = clockwise

const int QTIMiddle = 51;
const int QTILeft = 49;
const int QTIRight = 53;

const int COM_LED_PIN = 8;
const int COM_BUTTON_PIN = 2;

const int threshold = 20;
const int noMag = 522;
const int HALL_ANALOG_PIN = A2;
const int HALL_LED_PIN = 10;

/*
  -1 : none
  0 : hash
  1 : on left of line
  2 : on right of line
  3 : off line
  4 : on line
*/
int lastSense = -1;

void setup() {
  delay(1000);

  Serial.begin(9600);
  Serial2.begin(9600);

  servoLeft.attach(12);
  servoRight.attach(11);

  pinMode(COM_LED_PIN, OUTPUT);
  pinMode(COM_BUTTON_PIN, INPUT);

  pinMode(HALL_ANALOG_PIN, INPUT);
  pinMode(HALL_LED_PIN, OUTPUT);
}

void loop() {
  // true = on black line ~200 ish
  // false = not on black line ~30 ish
  // *** TODO *** create calibration technique
  int threshold = 80;
  bool middle = RCTime(QTIMiddle) > threshold;
  bool left = RCTime(QTILeft) > threshold;
  bool right = RCTime(QTIRight) > threshold;

  // Communication button press
  bool buttonPressed = digitalRead(COM_BUTTON_PIN);

  // Hall-effect sensor analog input
  int val = analogRead(A2);

  // ** Log QTI sensor data **
  // Serial.print("Middle: ");
  // Serial.println(middle);
  // Serial.print("left: ");
  // Serial.println(left);
  // Serial.print("right: ");
  // Serial.println(right);

  //Check if message should be sent
  if(Serial.available() || buttonPressed){
    char outgoing = Serial.read();
    if(buttonPressed){
      outgoing = 'f';
    }
    // Serial.print("Sending: ");
    // Serial.println(outgoing);
    Serial2.print(outgoing);
    delay(200);
  }

  //Check if there is an incoming message
  if(Serial2.available()){
    char incoming = Serial2.read();
    // Serial.print("Receiving: ");
    // Serial.println(incoming);
    digitalWrite(COM_LED_PIN, HIGH);
    delay(500);
  } else {
    digitalWrite(COM_LED_PIN, LOW);
  }

  // Hall-effect sensor led indicator 
  Serial.println(val); // Log sensor values
  if(val < noMag - threshold || val > noMag + threshold){
    digitalWrite(HALL_LED_PIN, HIGH);
    delay(100);
  } else {
    digitalWrite(HALL_LED_PIN, LOW);
  }

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

  // if((middle && left && right || lastSense == 0) ) {
  //   // on hash
  //   if(lastSense == 0){
  //     goForwardLeft(25);
  //     return;
  //   }
  //   lastSense = 0;
  //   stopMoving(1000);
  //   // goForward(50);
  //   goLeft(250);
  //   return;
  // }

  // off line
  // if(!middle && !left && !right) {
  //   goBackward(50);
  //   return;
  // }

  // delay(time);
}

void goForward(int time){
  // lastMove = 0;
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
  delay(time);
}

void goBackward(int time){
  servoLeft.writeMicroseconds(1450);
  servoRight.writeMicroseconds(1550);
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
