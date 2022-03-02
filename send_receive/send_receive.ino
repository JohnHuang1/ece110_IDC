#define Rx 17
#define Tx 16

const int LED_PIN = 8;
const int BUTTON_PIN = 2;

void setup() {
  //Setup Serial Monitors
  Serial.begin(9600);
  Serial2.begin(9600);
  //Setup output pin for LED
  pinMode(LED_PIN, OUTPUT);
  //Setup input pin for Button press
  pinMode(BUTTON_PIN, INPUT);
  delay(500);
}

void loop() {
  bool buttonPressed = digitalRead(BUTTON_PIN); // Check if button is pressed

  if(Serial.available() || buttonPressed){ //Check if message should be snse
    char outgoing = Serial.read();
    if(buttonPressed){
      outgoing = 'f';
    }
    Serial.print("Sending: ");
    Serial.println(outgoing);
    Serial2.print(outgoing);
  }

  if(Serial2.available()){
    char incoming = Serial2.read();
    Serial.print("Receiving: ");
    Serial.println(incoming);
    digitalWrite(LED_PIN, HIGH);
  } else {
    delay(100);
    digitalWrite(LED_PIN, LOW);
  }

  delay(50);
}
