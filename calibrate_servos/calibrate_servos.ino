# include <Servo.h>

Servo servoLeft; // higher number = counter clockwise
Servo servoRight; // lower number = clockwise


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  servoLeft.attach(12);
  servoRight.attach(11);
}

void loop() {
  // put your main code here, to run repeatedly:

  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);

  Serial.println("Loop");
  delay(1000);

}
