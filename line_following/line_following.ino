# include <Servo.h>

Servo servoLeft; // higher number = counter clockwise
Servo servoRight; // lower number = clockwise

int QTIMiddle = 51;
int QTILeft = 49;
int QTIRight = 53;

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
  
  if(middle && left && right) {
    stopMoving(1000);
    goForward(25);
    // goLeft(250);
    return;
  }

  // if(!middle && !left && !right) {
  //   goLeft(250);
  //   return;
  // }

  if(middle && !left && !right){
    goForward(time);
    return;
  }

  if(!left && right){
    goRight(time);
    return;
  }

  if(left && !right){
    goLeft(time);
    return;
  }

  // delay(time);
}

void goForward(int time){
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
  delay(time);
}

void goBackward(int time){
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
  delay(time);
}

void stopMoving(int time){
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(time);
}

void goLeft(int time){
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
