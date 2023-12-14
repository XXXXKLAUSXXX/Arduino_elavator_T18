// communication
#include <Wire.h>

int ChosenFloor = 0;
int LiveFloor = 0;

// key board
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 40, 22, 24, 26 };
byte colPins[COLS] = { 28, 30, 32, 34 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// code


void setup() {
  Wire.begin();
  Serial.begin(9600);
}



void loop() {
  // communication
  for (int i = 0; i <= 4; i++) {  // read the IR inputs from all the slaves, update the live floor
    requestFromSlave(i);
  }
  Serial.println();
  delay(5);

  for (int i = 0; i <= 4; i++) {  //transmit the live floor to all slaves.
    Wire.beginTransmission(i);
    Wire.write(LiveFloor);
    Wire.write(ChosenFloor);
    Wire.endTransmission();
  }
  // keypad
  // make this decide chosen floor 
  int customKey = customKeypad.getKey();
  switch (customKey) {
    case 49:
ChosenFloor = 1;
      break;
    case 50:
ChosenFloor = 2;
      break;
    case 51:
ChosenFloor = 3;
      break;
  }

  if (customKey) {
    Serial.println(customKey);
  }
  // moter
  int DeltaFloor = ChosenFloor - LiveFloor;
  if ((DeltaFloor) != 0) {
    // if DeltaFloor is a posetive number the elevator must go up
    if (DeltaFloor > 0) {
      //Moter up
    } else {
      //Moter down
    }
  }

}  //end of loop



void requestFromSlave(int slave) {
  Wire.requestFrom(slave, 1);  //request 1 byte

  int tempLF = Wire.read();  // read 1 byt of the slave and if it is not 0, we've found the live floor.
  int tempCH = Wire.read();  // If a button is pressed on a floor this will tell the master what floor it is
  if (tempLF > 0) {          // only update if the value is greater then 0
    LiveFloor = tempLF;
  }
  if (tempCH > 0) {  // only update if the value is greater then 0
    ChosenFloor = tempCH;
  }
  Serial.print(LiveFloor);
  return;
}
