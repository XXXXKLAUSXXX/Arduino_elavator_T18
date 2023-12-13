#include <Wire.h>

#define IRPIN 1
#define BTNPIN 2

#define LEDRED 8
#define LEDGREEN 9
#define BTNLED 10



int X = 1;

bool IR;
bool BTN_low;
bool Chosen_one;

void requestEvent(){
  Wire.write(BTN_low);
  Wire.write(IR);    
    Serial.println(Chosen_one);
  return;        
}

bool receiveEvent(){
  Chosen_one = Wire.read();
  return Chosen_one;
}

void setup() {

int Adress = 20 + X;
  Wire.begin(Adress);            
  Wire.onRequest(requestEvent);  
  Wire.onReceive(receiveEvent);

pinMode(IRPIN, INPUT);
pinMode(BTNPIN, INPUT);

pinMode(LEDRED, OUTPUT);
pinMode(LEDGREEN, OUTPUT);
pinMode(BTNLED, OUTPUT);
}


void loop() {
IR = digitalRead(IRPIN);
BTN_low = digitalRead(BTNPIN);

if(IR == 1){
digitalWrite(LEDRED, 1);
if(Chosen_one == true){
  digitalWrite(LEDGREEN, 1);}
}
else {
  digitalWrite(LEDRED, 0);
  digitalWrite(LEDGREEN, 0);
}


if(BTN_low == 1){digitalWrite(BTNPIN, 1);}
else {digitalWrite(BTNPIN, 0);}

if(Chosen_one == true){
  digitalWrite(BTNLED, 1);}
  else {
    digitalWrite(BTNLED, 0);
  }
}//end loop



