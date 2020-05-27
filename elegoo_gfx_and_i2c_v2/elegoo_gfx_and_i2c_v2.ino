#include <Elegoo_GFX.h>
#include <Elegoo_TFTLCD.h>
#include <Wire.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define  LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

//defining colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
int my_values[4]; //transmision

float width, height;
float x1,y1,x2,y2;
float read_value;

char message;
int i=0;
unsigned int colors[]={0,63488};
unsigned int color;
int c=0;
bool alarm=false;

unsigned long prev_t=0;
unsigned long now_t=0;

void setup() {
  Serial.begin(9600);
  Wire.begin(1);//initializing i2c communication
  Wire.onReceive(llegaDato); 
  //#ifdef USE_Elegoo_SHIELD_PINOUT
  //Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
  //#else
  //  Serial.println(F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
  //#endif
  //tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  
  tft.setRotation(3);
  width=tft.width();
  height=tft.height();
  tft.fillScreen(BLACK);
  x1=0; y1=height*.4; x2=10; y2=0;
}
void llegaDato() {
// Si hay dos bytes disponibles
 while (Wire.available()){
   //my_values[0] =Wire.read();
   //my_values[1]= Wire.read();
   my_values[i]=Wire.read();
   i=(i+1)%3;
  } 
}

void loop() {
  
  prev_t=millis();

  if(y2 < 60*.5/-1+height*.4){//alarm parameters
    //alarm=true;
    c=(c+1)%2;
    color=colors[c];
  }
  else{
    //alarm=false;
    color=colors[0];
  }
  
  //if(!alarm){color=colors[0];}
  
  if(x1>width || x2>width){//cleaning screen
    tft.fillScreen(BLACK);
    x1=0;
    x2=0;  
  }

  tft.drawLine(0,height*.15,width*.0125,height*.15,GREEN);//top line
  tft.drawLine(0,height*.75,width*.0125,height*.75,GREEN);//bottom line
  tft.drawLine(0,height*.4,width,height*.4,GREEN);//base line
  //tft.drawLine(0,60*.5/-1+height*.4,width,60*.5/-1+height*.4,WHITE);//threshold
  
  //Display parametro 1
  tft.fillRect(0,height*.8,width,height*.2,color);//keeps from overwriting in screen
  tft.setCursor(1,height*.87); tft.setTextSize(2);
  tft.print("P="); tft.print(my_values[0]);
  Serial.print("v0= "); Serial.println(my_values[0]);
  //Display parametro 2
  tft.setCursor(1+width*.33,height*.87); tft.setTextSize(2);
  tft.print("F="); tft.print(my_values[1]);
  Serial.print("v1= "); Serial.println(my_values[1]);
  //Display parametro 3
  //tft.fillRect(width*.66,height*.8,width*.33,height*.2,RED);

  if(my_values[2]==1){
    tft.fillRect(0,0,width,height*.15,color);
    tft.setCursor(0,0); tft.setTextSize(3);
    tft.println("Presion");
    y2=constrain(my_values[0]*.5/-1+height*.4, height*.15, height*.75);
    tft.drawLine(x1,y1,x2,y2,RED);
  }
  else{
    tft.fillRect(0,0,width,height*.15,color);
    tft.setCursor(0,0); tft.setTextSize(3);
    tft.println("Flujo");
    y2=constrain(my_values[1]*.5/-1+height*.4, height*.15, height*.75);
    tft.drawLine(x1,y1,x2,y2,CYAN);
  }
  
  x1=x2; y1=y2;
  x2+=2; //y2=constrain(my_values[0]*1/-1+height*.4, height*.15, height*.75);
  now_t=millis();
  Serial.print("t "); Serial.println(now_t-prev_t);
  prev_t=now_t;

}

