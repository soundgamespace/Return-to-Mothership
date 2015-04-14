//test code for LED's in paralled

int red[] = {2};

void setup() {
  // put your setup code here, to run once:
  pinMode(red[0], OUTPUT);
  digitalWrite(red[0], LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(red[0], LOW);
  delay(500);
  digitalWrite(red[0], HIGH);
  delay(500);
}
