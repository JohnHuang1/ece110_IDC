#include <SoftwareSerial.h>
const int TxPin = 6;
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

int pos = -1;
String lcdBottom = "";

void setup() {

pinMode(TxPin, OUTPUT);
digitalWrite(TxPin, HIGH);

  mySerial.begin(9600);
  delay(100);
  mySerial.write(12);
  mySerial.write(17);
  delay(100);
  mySerial.write(12);
  mySerial.write(17);
  delay(100);
  
}
void loop(){

//  int top = 0;
//  int bottom = 1;
//  for(int i = 0; i < 256){
//    top += 2;
//    bottom++;
//    LCD(""+ top, "" + bottom);
//  }
  if(lcdBottom.equals("")){
    lcdBottom = "Engineer the Tools of Scientific Discovery";
  }
  LCD("North America--e");

}

 //Str 1 ~ Continent
 //Str 2 ~ Grand Challenge
void LCD(String topString) {
  mySerial.write(12);   
  delay(5);
//  mySerial.write(128); //move to pos 0,0
  mySerial.print(topString);
  mySerial.write(13);
  mySerial.print(getLCDBottomString());
  mySerial.write(212);
  mySerial.write(220);
  delay(100);
}

String getLCDBottomString(){
  if(lcdBottom.length() < 16) return lcdBottom;
  pos ++;
  if(pos > lcdBottom.length() - 1) {
    pos = 0;
  }
  int endpos = pos + 16;
  if(endpos > lcdBottom.length()){
    endpos = lcdBottom.length();
  }
  return lcdBottom.substring(pos, endpos);
}
