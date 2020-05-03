#include <Wire.h>

volatile bool plot_v0=true;

void setup(){
  Wire.begin();
  pinMode(2,INPUT);
  attachInterrupt(digitalPinToInterrupt(2),change_plot,RISING);
}

void change_plot(){
  plot_v0=!plot_v0;
}

void loop(){
  Wire.beginTransmission(1);
  Wire.write(byte(75));
  Wire.write(byte(50));
  Wire.write(byte(plot_v0));
  Wire.endTransmission();
//  delay(500);
//  Wire.beginTransmission(2);
//  Wire.write('B');
//  Wire.endTransmission();
//  delay(500);  
}

