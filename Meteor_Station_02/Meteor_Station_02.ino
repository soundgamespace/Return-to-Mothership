
//code for meteor slave stations
#include <Wire.h>
#include "Adafruit_TCS34725.h"

int redLeds[] = {2, 3, 4, 5};
int greenLeds[] = {6, 7, 8, 9};
int blueLeds[] = {10, 11, 12, 13};

boolean redStates[] = {0, 1, 0, 1};
boolean greenStates[] = {0, 1, 0, 1};
boolean blueStates[] = {0, 1, 0, 1};

unsigned long time;
unsigned long interval = 500;

byte colorMode = 2;
byte gammatable[256];

// set to false if using a common cathode LED
#define commonAnode true
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  //slave address of 1
  Wire.begin(1);
  //forward message to triggerEvent function.
  Wire.onReceive(triggerEvent);
  //set our outputs
  delay(250);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
  }
  for (int i = 0; i < 4; i++) {
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
  //Serial.print("\t");
  //Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  Serial.println();
  Serial.print((int)r ); Serial.print(" "); Serial.print((int)g); Serial.print(" ");  Serial.println((int)b );

  if (r > 140 && colorMode == 1 && r > b + g) {
    //if red is detected
    Serial.println("RED DETECTED");
    colorMode = 0;
  }
  else if (g > 140 && colorMode == 1 && g > b + r) {
    //if green is detected
    Serial.println("GREEN DETECTED");
    colorMode = 0;
  }
  else if (b > 140 && colorMode == 1 && b > r + g) {
    //if blue is detected
    Serial.println("BLUE DETECTED");
    colorMode = 0;
  }
}

void triggerEvent(int port) {
  if (Wire.available()) {
    while (Wire.available()) {
      //setMeteor(Wire.read());
      colorMode = Wire.read();
      meteorPulse(colorMode);
    }
  }
}

void meteorPulse(byte _colorMode) {
  Serial.print("Color Mode : ");
  Serial.println(_colorMode);
  if (_colorMode == 0) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], LOW);
      digitalWrite(greenLeds[i], LOW);
      digitalWrite(blueLeds[i], LOW);
    }
  }
  else if (_colorMode == 1) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(greenLeds[i], LOW);
      digitalWrite(blueLeds[i], LOW);
      redStates[i] = !redStates[i];
      digitalWrite(redLeds[i], redStates[i]);
    }
  }
  else if (_colorMode == 2) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], LOW);
      digitalWrite(blueLeds[i], LOW);
      greenStates[i] = !greenStates[i];
      digitalWrite(greenLeds[i], greenStates[i]);
    }
    //Serial.print("Green States : ");
    //Serial.print(greenStates[0]);
    //Serial.print(greenStates[1]);
    //Serial.print(greenStates[2]);
    //Serial.print(greenStates[3]);
  }
  else if (_colorMode == 3) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(redLeds[i], LOW);
      digitalWrite(greenLeds[i], LOW);
      blueStates[i] = !blueStates[i];
      digitalWrite(blueLeds[i], blueStates[i]);
    }
  }
}

void loop() {
  time = millis();
  while (1) {
    readColors();
    if (time + interval < millis()) {
      int num = random(0, 11);
      if (num == 7) {
        colorMode = random(0, 4);
      } 
      meteorPulse(colorMode);
      //delay(40);
      time = millis();
    }

  }
}
