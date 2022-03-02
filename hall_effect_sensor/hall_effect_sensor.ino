#define hall_sensor 2

int threshold = 20;
int noMag = 522;
const int HALL_OUTPUT_PIN = 10;

void setup() {
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
}
