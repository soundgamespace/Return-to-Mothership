
//code for meteor slave stations
#include <Wire.h>
#include "Adafruit_TCS34725.h"

int redLeds[] = {2, 3, 4, 5};
int greenLeds[] = {6, 7, 8, 9};
int blueLeds[] = {10, 11, 12, 13};

boolean redStates[4];
boolean greenStates[4];
boolean blueStates[4];

unsigned long time;
unsigned long interval = 333;

int colorMode = 0;
byte gammatable[256];

// set to false if using a common cathode LED
#define commonAnode true

void setup() {
  //slave address of 1
  Wire.begin(1);
  //forward message to triggerEvent function.
  Wire.onReceive(triggerEvent);
  //set our outputs
  for (int i = 0; i < 3; i++) {
    pinMode(redLeds[i], OUTPUT);
    pinMode(greenLeds[i], OUTPUT);
    pinMode(blueLeds[i], OUTPUT);
    digitalWrite(redLeds[i], LOW);
    digitalWrite(greenLeds[i], LOW);
    digitalWrite(blueLeds[i], LOW);
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
}

void readColors() {
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);// turn on LED
  delay(60);  // takes 50ms to read
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(false);  // turn off LED
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  if (r > 140 && colorMode == 1 && r > b + g) {
    //if red is detected
    colorMode = 0;
  }
  else if (g > 140 && colorMode == 1 && g > b + r) {
    //if green is detected
    colorMode = 0;
  }
  else if (b > 140 && colorMode == 1 && b > r + g) {
    //if blue is detected
    colorMode = 0;
  }
}

void triggerEvent(int port) {
  if (Wire.available()) {
    while (Wire.available()) {
      //setMeteor(Wire.read());
    colorMode = Wire.read()  
    meteorPulse(colorMode);
    }
  }
}

void meteorPulse(int _colorMode) {
  if (_colorMode == 0) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], LOW);
      digitalWrite(greenLeds[i], LOW);
      digitalWrite(blueLeds[i], LOW);
      redStates[i] = 0;
      greenStates[i] = 0;
      blueStates[i] = 0;
    }
  }
  else if (_colorMode == 1) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(greenLeds[i], LOW);
      digitalWrite(blueLeds[i], LOW);
      greenStates[i] = 0;
      blueStates[i] = 0;
      redStates[i] != redStates[i];
      digitalWrite(redLeds[i], redStates[i]);
    }
  }
  else if (_colorMode == 2) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], LOW);
      digitalWrite(blueLeds[i], LOW);
      redStates[i] = 0;
      blueStates[i] = 0;
      greenStates[i] != greenStates[i];
      digitalWrite(greenLeds[i], greenStates[i]);
    }
  }
  else if (_colorMode == 3) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], LOW);
      digitalWrite(greenLeds[i], LOW);
      redStates[i] = 0;
      greenStates[i] = 0;
      blueStates[i] != blueStates[i];
      digitalWrite(blueLeds[i], blueStates[i]);
    }
  }
}

void loop() {
  time = millis();
  while (1) {
    readColors();
    if (time + interval < millis()) {
      meteorPulse(colorMode);
      time = millis();
    }
  }
}
