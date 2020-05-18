int relay_pin=40;
volatile byte relay_state=LOW;
unsigned long prev_t, now_t;

void setup() {
  pinMode(relay_pin,OUTPUT);
  digitalWrite(relay_pin,relay_state);
  prev_t=0;
}

void loop() {

  digitalWrite(relay_pin, relay_state);
  now_t=millis();
  if(now_t-prev_t >= 5000){
    relay_state=!relay_state;
    prev_t=now_t;
  }
}
