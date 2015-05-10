//code for meteor slave stations
#include <Wire.h>

unsigned long currentTime;
unsigned long lastTime[8];
unsigned long interval = 50;

int meteorStates[8];

void setup() {
  Serial.begin(9600);
  Wire.begin(0);
}

void maxReceive(){
  if (Serial.available()){
    while (Serial.available()){
     int meteorNum = Serial.read();
     int color = Serial.read();
     turnOn(meteorNum, color); 
    }
  }
}

void turnOn(int meteorNum, int color){
 Wire.beginTransmission(meteorNum);
 Wire.write(color);
 Wire.endTransmission();
 Serial.print("Meteor Number : ");
 Serial.print(meteorNum);
 Serial.print(" Color is : ");
 Serial.println(color);
}

void loop() {
  //maxReceive();
  testI2C();
  currentTime = millis();
  for (int i = 0; i < 8; i++){
    if(lastTime[i] + interval < currentTime){
     readMeteor(i); 
    }
  }
  delay(1000);
}

void testI2C(){
 turnOn(random(1,8), random(1,3)); 
}

void readMeteor(int slaveID){
  
  Wire.beginTransmission(slaveID);
  int available = Wire.requestFrom(slaveID, 1);
  //
  if (available == 1){
    if(Wire.read() == 255){
       meteorStates[slaveID - 1] = 0;
       tellMax(slaveID);
    }    
  }
}

void tellMax(int _slaveID){
   Serial.write(_slaveID); 
}

