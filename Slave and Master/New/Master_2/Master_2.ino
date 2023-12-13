#include <Wire.h>

int ChosenFloor = 0;
int LiveFloor = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}



void loop() {
  for(int i = 0; i <= 4; i++){ // read the IR inputs from all the slaves, update the live floor
  requestFromSlave(i);
  }
  Serial.println();
  delay(500);

  ChosenFloor = (ChosenFloor + 1) % 4; // cycle 1-4, for testing purposes

for(int i = 0; i <= 4; i++) { //transmit the live floor to all slaves.
    Wire.beginTransmission(i);
    Wire.write(LiveFloor);
   Wire.write(ChosenFloor);
    Wire.endTransmission();
  }
}



 void requestFromSlave(int slave) {
  Wire.requestFrom(slave, 1); //request 1 byte

  int temp = Wire.read(); // read 1 byt of the slave and if it is not 0, we've found the live floor.
  if (temp > 0) {
    LiveFloor = temp;
  }
  Serial.print(LiveFloor);
  return;
}