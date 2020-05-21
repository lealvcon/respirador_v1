#include <DueTimer.h> //libreria para manejar interrupciones de tiempo 
#include <Average.h>  //libreria para manejar arreglos circulares y operaciones estadisticas
#include <Wire.h>     //libreria para comunicacion i2c


int address1 = 0x49;  //hexadecimal address of sensor Zephir//address from data sheet
volatile float presion;
unsigned long flujo;

const int plot_button_pin = 3;
volatile bool var_plot = true;
// declaración de tamaño de buffer para cada sensor

Average<float> pr(10);

//funcion para cambiar de estado en la pantalla
void change_plot() {
  var_plot = !var_plot;
}

// funciones dentro de las interrupciones
void sensor1() { //muestrea y almacena los datos de una entrada digital
  presion = map(analogRead(0), 0, 400, 0, 128);
  // Serial.println(map(analogRead(0),0,400,0,128));
}
void sensor2() {
  flujo = read_flow(address1) - 6;
  //Serial.println(flujo);

}

// funcion para lectura de flujo
int read_flow(int address) {
  //start the communication with IC with the address xx
  Wire.beginTransmission(address);
  //send a bit and ask for register zero
  Wire.write(0);
  //end transmission
  Wire.endTransmission();
  //request 1 byte from address xx
  Wire.requestFrom(address, 1);
  //wait for response
  while (Wire.available() == 0);
  //put the flow in variable c
  int c = Wire.read();
  return c;
}
void setup() {
  pinMode(plot_button_pin, INPUT);
  Serial.begin(9600);
  //inicializacion de valores del buffer
  for (int i = 0; i < 10; i++) {
    pr.push(0);
  }
  Wire.begin();     // create a wire object
  //declaracion de interrupciones para lectura de señales de sensores
  attachInterrupt(digitalPinToInterrupt(plot_button_pin), change_plot, RISING);
  Timer1.attachInterrupt(sensor1).setFrequency(8).start();
  Timer2.attachInterrupt(sensor2).setFrequency(8).start();

}

void loop() {
  Serial.println(presion);
  Serial.println(flujo);
  //transmision a esclavo2
  Wire.beginTransmission(2);  // Transmite al Esclavo 2
  Wire.write(byte(presion));
  Wire.write(byte(flujo));

  Wire.endTransmission();
  //transmision a esclavo 1
  Wire.beginTransmission(1);
  Wire.write(byte(presion));
  Wire.write(byte(flujo));
  Wire.write(byte(var_plot));//envialo al ultimo we
  Wire.endTransmission();
Serial.println("boton   ");
Serial.println(var_plot);
     delay(500);
}
