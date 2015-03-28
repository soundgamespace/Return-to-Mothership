#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)

int redPins[] = {4, 7, 10};
int greenPins[] = {5, 8, 11};
int bluePins[] = {6, 9, 12};

//#define irpin A0
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];
int irValue;
int outColor; //1 is red, 2 is green, 3 is blue, 4 is white

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {

  Serial.begin(9600);
  Serial.println("Sonic-Hammer Test!");
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  // use these three pins to drive an LED
  for (int i = 0; i < 3; i++) {
    pinMode(redPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
    pinMode(bluePins[i], OUTPUT);
  }
  //pinMode(irpin, OUTPUT);
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


void loop() {
  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(false);  // turn off LED
  //Serial.print("C:\t"); Serial.print(clear);
  //Serial.print("\tR:\t"); Serial.print(red);
  //Serial.print("\tG:\t"); Serial.print(green);
  //Serial.print("\tB:\t"); Serial.print(blue);
  // Figure out some basic hex code for visualization
  //irValue = analogRead(irpin);
  //Serial.println("---------------------");
  //Serial.println(irValue);
  //Serial.println("---------------------");
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  //Serial.print("\t");
  //Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  //Serial.println();
  //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );
  if (r > 50 && outColor != 1 && r > b + g) {
    Serial.println("RED");
    for (int i = 0; i < 3; i++) {
      digitalWrite(redPins[i], LOW);
      digitalWrite(greenPins[i], HIGH);
      digitalWrite(bluePins[i], HIGH);
    }
    outColor = 1;
  }
  else if (g > 50 && outColor != 2 && g > r + b) {
    for (int i = 0; i < 3; i++) {
    Serial.println("GREEN");
      digitalWrite(redPins[i], HIGH);
      digitalWrite(greenPins[i], LOW);
      digitalWrite(bluePins[i], HIGH);\   
    }
    outColor = 2;
  }
  else if (b > 50 && outColor != 3 && b > r + g) {
    for (int i = 0; i < 3; i++) {
    Serial.println("BLUE");
      digitalWrite(redPins[i], HIGH);
      digitalWrite(greenPins[i], HIGH);
      digitalWrite(bluePins[i], LOW);

    }
    outColor = 3;
  }
  else if (outColor == 0) {
    Serial.println("WHITE");
    for (int i = 0; i < 3; i++) {
    analogWrite(redPins[i], HIGH);
      analogWrite(greenPins[i], HIGH);
      analogWrite(bluePins[i], HIGH);
    }
    outColor = -1;
  }
}

