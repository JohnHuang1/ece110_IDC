

void setup() {
  Serial.begin(9600);

}
int values[6] = {-1,-1,-1,-1,-1,-1};
char arr[12]= {'a','b','c','d','e','f','g','h','i','j','k','l'};
//a = 97
//l = 108

bool done = false;

void loop() {
  if(done) return;
  for(int i = 0; i < 12; i++){
    int val = arr[i];
    Serial.print(arr[i]);
    Serial.print("-");
    Serial.print(val);
    Serial.print("-");
    Serial.print((val + 1) % 2);
    Serial.print("-");
    Serial.println((val - 97) / 2);
  }
  done = true;
  delay(500);

}

void stuff(int val){
    Serial.print("-");
    Serial.print(val);
    Serial.print("-");
    Serial.print((val + 1) % 2);
    Serial.print("-");
    Serial.println((val - 97) / 2);
}
