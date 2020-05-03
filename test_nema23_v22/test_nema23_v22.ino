//https://www.youtube.com/watch?v=d3YCz2N5CRI
//USE MICROSTEP 4 (1.8°/step )= (0.45°/step)
//common cathode connection
//analog and digital pins from 0 to 3.3 v

// defines pins numbers
const int stepPin = 5; 
const int dirPin = 6; 
const int enPin = 8;
const int buzzer_pin=2;
volatile int steps_taken=0; //steps taken from origin
const int button=12;
unsigned long now_time, prev_time;

//I:E control variables
volatile int c=2;
const int s1_pin=10;
const int s3_pin=9;

//first parameter
volatile int vol_steps;
const int vol_interrupt_pin=4;
const int vol_pot= A0;

//second parameter
volatile int bpm_delay;
const int bpm_interrupt_pin=3;
const int bpm_pot= A2;

//ISR's
void ratio_1(){c=1;}

void ratio_3(){c=3;}

void bpm_interrupt(){
  bpm_delay=analogRead(bpm_pot);
  buzz(1);
  //Serial.print("read value= "); Serial.println(bpm_delay);
  Serial.print("bpm delay= "); Serial.println(bpm_delay);//check mapping values
}

void vol_interrupt(){
  vol_steps=analogRead(vol_pot);  
  buzz(1);
  //Serial.print("read value= "); Serial.println(ins_delay);
  Serial.print("vol steps= "); Serial.println(vol_steps);//chek mapping values
}

void homing(){
  steps_taken=0;
}


void buzz(int seconds){
    for(int i=0; i<seconds; i++){
      digitalWrite(buzzer_pin,HIGH);
      for(int k=0; k<50; k++) {delayMicroseconds(10000);}
      digitalWrite(buzzer_pin,LOW);
      for(int k=0; k<50; k++){delayMicroseconds(10000);}
    }
}



void setup() {
  Serial.begin(9600);
  analogReadResolution(10);
  pinMode(buzzer_pin,OUTPUT);
  digitalWrite(buzzer_pin,LOW);
  pinMode(s1_pin,INPUT);
  pinMode(s3_pin,INPUT);
  pinMode(vol_interrupt_pin,INPUT);
  pinMode(bpm_interrupt_pin,INPUT);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(button,INPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  attachInterrupt(digitalPinToInterrupt(vol_interrupt_pin),vol_interrupt,RISING);
  attachInterrupt(digitalPinToInterrupt(bpm_interrupt_pin),bpm_interrupt,RISING);
  attachInterrupt(digitalPinToInterrupt(s1_pin),ratio_1, RISING);
  attachInterrupt(digitalPinToInterrupt(s3_pin),ratio_3, RISING);
  vol_steps=500;
  //attachInterrupt(digitalPinToInterrupt(12),homing,RISING);
  //digitalWrite(dirPin,LOW);
//  for(int s=0; s<800; s++){//searching for home
//    digitalWrite(stepPin,HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin,LOW);
//    delayMicroseconds(500);
//    if(digitalRead(button)==HIGH){
//      digitalWrite(button,LOW);
//      break;
//    }
//  }
  now_time=millis();
}

void move_ccw(int steps){
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
    steps_taken=steps_taken-1;
    delay(c*5);//tiempo entre paso y paso hacia atrÃ¡s permite regular tiempo de espiraciÃ³n?
    //delay(map(ins_delay,0,1023,0.1,100));
  }
}

void move_cw(int steps){

  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
    steps_taken=steps_taken+1;
    delay(1);//tiempo de inspiraciÃ³n?
//    if(steps_taken==400){//lÃ­mite de pasos para delimitar volumen entregado?
//      move_ccw(400);
//      break;
//    }
    if(steps_taken==map(vol_steps,0,1023,10,800)){
      move_ccw(map(vol_steps,0,1023,10,800));
      break;
    }
  }
}

void loop() {
  
    if(digitalRead(s1_pin)==LOW && digitalRead(s3_pin)==LOW){c=2;}
    Serial.print("c= "); Serial.println(c);
    Serial.print("steps_taken= "); Serial.println(steps_taken);
    Serial.print("vol_steps= "); Serial.println(vol_steps);
    move_cw(800);
    //delay(10);
    delay(map(bpm_delay,0,1023,1,1000));//controlar tiempo entre inspiraciÃ³n y espiraciÃ³n?

}
