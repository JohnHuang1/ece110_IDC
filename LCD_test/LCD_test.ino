#include <SoftwareSerial.h>
const int TxPin = 6;
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

void setup() {

pinMode(TxPin, OUTPUT);
digitalWrite(TxPin, HIGH);

  mySerial.begin(9600);
  delay(100);
  mySerial.write(12);
  mySerial.write(17);
  delay(5);
  mySerial.print("john");
  mySerial.write(13);
  mySerial.print("huang");
  mySerial.write(212);
  mySerial.write(220);
  delay(3000);
  mySerial.write(18);

}
void loop(){

}

 //Str 1 ~ Continent
 //Str 2 ~ Grand Challenge
 
void LCD(String continent, String challenge) {
  while(1){
      
  delay(100);
  mySerial.write(12);
  mySerial.write(17);
  delay(5);
  mySerial.print(continent);
  mySerial.write(13);
  mySerial.print(challenge);
  mySerial.write(212);
  mySerial.write(220);
  delay(3000);
  mySerial.write(18);
  }
}
