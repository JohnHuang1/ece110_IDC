#define hall_sensor 2

int threshold = 5;
int noMag = 323;

void setup() {
  Serial.begin(9600);
  pinMode(A2, INPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  int val = analogRead(A2);
  Serial.println(val);
  if(val < noMag - threshold || val > noMag + threshold){
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(5, LOW);
  }
  delay(100);
}
