#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 9
#define greenpin 10
#define bluepin 11
#define irpin 14
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode false

// our RGB -> eye-recognized gamma color
byte gammatable[256];
int irValue;
int outColor; //1 is red, 2 is green, 3 is blue, 4 is white

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  Serial.println("Color View Test!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(irpin, OUTPUT);
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
    //Serial.println(gammatable[i]);
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
  
  irValue = analogRead(irpin);
  
  Serial.println(irValue);
  
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
  if (r > 150 && outColor != 1 && r > b + g) {
    Serial.println("RED");
    analogWrite(redpin, 1023);
    analogWrite(greenpin, 0);
    analogWrite(bluepin, 0);
    outColor = 1;
  }
  else if (g > 150 && outColor != 2 && g > r + b) {
    Serial.println("GREEN");
    analogWrite(redpin, 0);
    analogWrite(greenpin, 1023);
    analogWrite(bluepin, 0);
    outColor = 2;
  }
  else if (b > 150 && outColor != 3 && b > r + g) {
    Serial.println("BLUE");
    analogWrite(redpin, 0);
    analogWrite(greenpin, 0);
    analogWrite(bluepin, 1023);
    outColor = 3;
  }
  /*
  else if (blue > 16000 && red > 16000 && green > 16000 && outColor != 4) {
    Serial.println("WHITE");
    analogWrite(redpin, 0);
    analogWrite(greenpin, 0);
    analogWrite(bluepin, 1023);
    outColor = 4;
  }
  */
  else if (outColor == 0) {
    Serial.println("WHITE");
    analogWrite(redpin, 1023);
    analogWrite(greenpin, 1023);
    analogWrite(bluepin, 1023);
    outColor = -1;
  }
}

