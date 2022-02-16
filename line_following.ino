# include <Servo.h>

Servo servoLeft;
Servo servoRight;

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
  pinMode(QTIMiddle, OUTPUT);
  digitalWrite(QTIMiddle, HIGH);
  pinMode(QTILeft, OUTPUT);
  digitalWrite(QTILeft, HIGH);
  pinMode(QTIRight, OUTPUT);
  digitalWrite(QTIRight, HIGH);
  delay(1);

  pinMode(QTIMiddle, INPUT);
  pinMode(QTILeft, INPUT);
  pinMode(QTIRight, INPUT);
  delay(1);

  int middle = digitalRead(QTIMiddle);
  int left = digitalRead(QTILeft);
  int right = digitalRead(QTIRight);

  Serial.print("Middle = ");
  Serial.println(middle);
  Serial.print("left = ");
  Serial.println(left);
  Serial.print("right = ");
  Serial.println(right);
  delay(100);
}

void goLeft(int time){
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1425);
  delay(time);
}

void goRight(int time){
  servoLeft.writeMicroseconds(1575);
  servoRight.writeMicroseconds(1475);
  delay(time);
}
