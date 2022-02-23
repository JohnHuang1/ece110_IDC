# include <Servo.h>

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

void setup() {
  tone(4, 3000, 3000);
  delay(1000);

  Serial.begin(9600);

  servoLeft.attach(12);
  servoRight.attach(11);

}

void loop() {
  // true = on black line ~200 ish
  // false = not on black line ~30 ish
  bool middle = RCTime(QTIMiddle) > 100;
  bool left = RCTime(QTILeft) > 100;
  bool right = RCTime(QTIRight) > 100;

  Serial.print("Middle: ");
  Serial.println(middle);
  Serial.print("left: ");
  Serial.println(left);
  Serial.print("right: ");
  Serial.println(right);
  int time = 1;
  
  // if(middle && left && right) {
  //   // on hash
  //   if(lastSense == 0){
  //     goForward(25);
  //     return;
  //   }
  //   lastSense = 0;
  //   stopMoving(1000);
  //   goForward(50);
  //   // goLeft(250);
  //   return;
  // }

  // if(!middle && !left && !right) {
  //   // off line
  //   // lastSense = 3;
  //   switch(lastSense){
  //     case 0: // on hash
  //       goBackwardRight(time);
  //       break;
  //     case 1: //on left
  //       goLeft(time);
  //       break;
  //     case 2: //on right
  //       goRight(time);
  //       break;
  //     case 3:  // off line
  //       goBackward(time);
  //       break;
  //     case 4: // on line
  //       goForwardLeft(time);
  //       break;
  //     default:
  //       stopMoving(time);
  //       break;
  //   }
  //   // goLeft(time);
  //   return;
  // }

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

  if((middle && left && right) || lastSense == 0) {
    // on hash
    if(lastSense == 0){
      goForwardLeft(25);
      return;
    }
    lastSense = 0;
    stopMoving(1000);
    goForwardLeft(50);
    // goLeft(250);
    return;
  }

  // delay(time);
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
