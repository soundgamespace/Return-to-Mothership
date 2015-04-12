#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

uint16_t r, g, b, c, colorTemp, lux = 0;  // define values for color sensor readings, initialize to 0

int sensorA = 2;  // digital pin 7 powers sensor A
int sensorB = 3;  // digital pin 8 powers sensor B

void setup(void) {
  Serial.begin(9600);
  pinMode(sensorA, OUTPUT);  // set pin to power sensorA
  pinMode(sensorB, OUTPUT);  // set pin to power sensorB
  digitalWrite(sensorA, LOW);
  digitalWrite(sensorB, LOW);
}

void loop(void) {

  // Using two FOR loops to collect samples from independent color sensors.  Could probably condense into further functions, maybe in next revision.

  // loop to sample sensorB 5 times. NOTE: the first 2 samples (i = 0, 1) will be incorrect; samples i = 2,3,4 give proper readings.
  for (int i = 0; i < 1; i++) {
    Serial.println("Sensor B");
    sensorON(sensorB);
    delay(100);
  }

  digitalWrite(sensorB, LOW);

  delay(1000);

  for (int j = 0; j < 1; j++) {
    Serial.println("Sensor A");
    sensorON(sensorA);
    delay(100);
  }

  digitalWrite(sensorA, LOW);

  delay(3000);

}

// create function to call the sensor depending on the pin desired
void sensorON(int pin) { // "pin" is determined in the main loop, and is the pin for the sensor that is getting powered on

  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);  // initialize sensor values each time the function gets called

  digitalWrite(pin, HIGH);  // set "pin" to high, powering the sensor

  // collect data from color sensor
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  // display data
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}
