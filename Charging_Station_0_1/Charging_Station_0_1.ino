//sketch for LED charging station

#define redpin 9
#define greenpin 10
#define bluePin 11

void setup() {
  // set ledpins to output
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
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
