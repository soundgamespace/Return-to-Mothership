
//code for meteor slave stations
#include <Wire.h>
#include "Adafruit_TCS34725.h"

int greenLeds[] = {2, 3, 4, 5};
int blueLeds[] = {6, 7, 8, 9};
int redLeds[] = {10, 11, 12, 13};

int counter = 0;

boolean redStates[] = {1, 1, 0, 0};
boolean greenStates[] = {1, 1, 0, 0};
boolean blueStates[] = {1, 1, 0, 0};

unsigned long time;
unsigned long interval = 250;

byte colorMode = 0;
byte gammatable[256];

// set to false if using a common cathode LED
#define commonAnode false
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(redLeds[i], OUTPUT);
    pinMode(greenLeds[i], OUTPUT);
    pinMode(blueLeds[i], OUTPUT);
    digitalWrite(redLeds[i], HIGH);
    digitalWrite(greenLeds[i], HIGH);
    digitalWrite(blueLeds[i], HIGH);
  }
  delay(120);
  if (tcs.begin()) {
    //Serial.println("Found sensor");
    flashWhite();
    delay(500);
  }
  else {
    //Serial.println("No TCS34725 found ... check your connections");
  }

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
  }
  delay(1000);
  flashWhite();
  flashWhite();
}

void readColors() {
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);// turn on LED
  delay(60);  // takes 50ms to read
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(false);  // turn off LED
  uint32_t sum = clear;
  //Serial.print(red ); Serial.print(" "); Serial.print(green); Serial.print(" ");  Serial.println(blue );

  if (red > 110 && colorMode == 1 && red > blue + green) {
    //if red is detected
    Serial.print(0xff);
    colorMode = 0;
    flashWhite();
    flashWhite();
    delay(300);
  }
  else if (green > 110 && colorMode == 2 && green > blue + red) {
    //if green is detected
    Serial.println(0xff);
    colorMode = 0;
    flashWhite();
    flashWhite();
    delay(300);
  }
  else if (blue > 110 && colorMode == 3 && blue > red + green) {
    //if blue is detected
    Serial.println(0xff);
    colorMode = 0;
    flashWhite();
    flashWhite();
    delay(300);
  }
}
/*
void triggerEvent(int port) {
  if (Wire.available()) {
    while (Wire.available()) {
      //setMeteor(Wire.read());
      colorMode = Wire.read();
      meteorPulse(colorMode);
    }
  }
}
*/
void meteorPulse() {
  //Serial.print("Color Mode : ");
  //Serial.println(_colorMode);
  if (colorMode == 0) {
    for (int i = 0; i < 4; i++) {

      digitalWrite(redLeds[i], HIGH);
      digitalWrite(greenLeds[i], HIGH);
      digitalWrite(blueLeds[i], HIGH);
    }
  }
  else if (colorMode == 1) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(greenLeds[i], HIGH);
      digitalWrite(blueLeds[i], HIGH);
      redStates[i] = !redStates[i];
      digitalWrite(redLeds[i], redStates[i]);
    }
  }
  else if (colorMode == 2) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], HIGH);
      digitalWrite(blueLeds[i], HIGH);
      greenStates[i] = !greenStates[i];
      digitalWrite(greenLeds[i], greenStates[i]);
    }
  }
  else if (colorMode == 3) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], HIGH);
      digitalWrite(greenLeds[i], HIGH);
      blueStates[i] = !blueStates[i];
      digitalWrite(blueLeds[i], blueStates[i]);
    }
  }
}

void flashWhite() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(redLeds[i], LOW);
    digitalWrite(greenLeds[i], LOW);
    digitalWrite(blueLeds[i], LOW);

  }
  delay(100);
  for (int i = 0; i < 4; i++) {
    digitalWrite(redLeds[i], HIGH);
    digitalWrite(greenLeds[i], HIGH);
    digitalWrite(blueLeds[i], HIGH);

  }
  delay(100);
  for (int i = 0; i < 4; i++) {
    digitalWrite(redLeds[i], LOW);
    digitalWrite(greenLeds[i], LOW);
    digitalWrite(blueLeds[i], LOW);

  }
  delay(100);
  for (int i = 0; i < 4; i++) {
    digitalWrite(redLeds[i], HIGH);
    digitalWrite(greenLeds[i], HIGH);
    digitalWrite(blueLeds[i], HIGH);

  }
}
/*
void proof_of_concept()
{
  time = millis();
  while (1) {
    readColors();
    if (time + interval < millis()) {
      //int num = random(0, 11);
      meteorPulse(colorMode);
      time = millis();
      if (colorMode == 0) {
        colorMode = random(1, 3);
      }
      if (counter == 59) {

        colorMode = (colorMode + 1) % 4;
      }
      counter = (counter + 1) % 60;
    }
  }
}
*/
void incomming() {
  while (colorMode != 0) {
    readColors();
    if (time + interval < millis()) {
      meteorPulse();
      time = millis();
    }
  }
}

void serialPoller() {
  while (Serial.available()) {
    if (Serial.available()) {
      colorMode = Serial.read();
      //Serial.print(colorMode);
      incomming();
    }
  }
}


void loop() {
  serialPoller();
}
