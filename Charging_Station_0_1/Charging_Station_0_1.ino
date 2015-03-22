//sketch for LED charging station

#define redpin 9
#define greenpin 10
#define bluein 11
#define irpin 12

void setup() {
  // set ledpins to output
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
    pinMode(irpin, OUTPUT);
    digitalWrite(irPin, HIGH);
}

void loop() {
  
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
