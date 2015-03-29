//sketch for LED charging station

#define redpin 3
#define greenpin 5
#define bluein 6
//#define irpin 12

void setup() {
  // set ledpins to output
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  //  pinMode(irpin, OUTPUT);
  //digitalWrite(irPin, HIGH);
}

void dorment() {
  for (int i = 0; i < 1024; i = i + 4) {
    analogWrite(redpin, i);
    analogWrite(bluepin, 1024 - i);
    delay(4);
  }
  for (int i = 0; i < 1024; i = i + 4) {
    analogWrite(greenpin, i);
    analogWrite(redpin, 1024 - i);
    delay(4);
  }
  for (int i = 0; i < 1024; i = i + 4) {
    analogWrite(bluepin, i);
    analogWrite(greenpin, 1024 - i);
    delay(4);
  }
}

void blinker(){
 digitalWrite(bluepin, LOW);
  digitalWrite(redpin, HIGH);
  delay(500);

  digitalWrite(redpin, LOW);
  digitalWrite(greenpin, HIGH);
  delay(500);

  digitalWrite(greenpin, LOW);
  digitalWrite(bluepin, HIGH);
  delay(500); 
}

void loop() {
blinker();
dorment();
  
}
