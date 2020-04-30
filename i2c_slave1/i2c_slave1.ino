#include <Wire.h>

const byte slaveID=1;

void setup(){
  Wire.begin(slaveID);
  Wire.onReceive(receiveEvent);

  
}

void loop(){
  
}

void receiveEvent(int howmany){
  char inchar;
  while(wire.available()>0){
    inchar=wire.read();
    Serial.println(inchar);    
  }
}

