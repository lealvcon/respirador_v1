#include <Wire.h>

void setup(){
  Wire.begin();
}

void loop(){
  Wire.beginTransmission(1);
  Wire.write(byte(random(0,125)));
  Wire.write(byte(random(0,125)));
  Wire.endTransmission();
//  delay(500);
//  Wire.beginTransmission(2);
//  Wire.write('B');
//  Wire.endTransmission();
//  delay(500);  
}

