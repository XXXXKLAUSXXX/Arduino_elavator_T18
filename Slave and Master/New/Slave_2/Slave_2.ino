#include <Wire.h>

#define slave 3
#define IRx 7
#define BUTTONx 999
#define LEDGREEN 5
#define LEDRED 6

int LiveFloor = 0;
int ChosenFloor = 0;
bool IR = 0;
bool BUTTON = 0;

//7SEG display
// ST_CP pin 12
const int latchPin = 10;
// SH_CP pin 11
const int clockPin = 11;
// DS pin 14
const int dataPin = 12;
// Patterns for characters 0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F
int datArray[4] = {
  //0
  B10111111,
  //1
  B00100100,
  //2
  B01011110,
  //3
  B01110110
};

void setup() {
  Wire.begin(slave);
  Serial.begin(9600);
  Wire.onRequest(requestEvent);
  Wire.onReceive(recieveEvent);

    // 7SEG
    // Setup pins as Outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

      digitalWrite(latchPin, LOW);

    // Shift out the bits
    shiftOut(dataPin, clockPin, MSBFIRST, datArray[1]);

    // ST_CP HIGH change LEDs
    digitalWrite(latchPin, HIGH);
}

void loop() {
  IR = !digitalRead(IRx);
  BUTTON = !digitalRead(BUTTONx);

  if (digitalRead(IR) == 0) {  // now this nested if statement toggles the leds on if the IR is triggered. If this floor is the Chosen Floor the green led will also turn on.
    digitalWrite(LEDRED, HIGH);
    Serial.println("LEDFIRE RED ");
    if (slave == ChosenFloor) {
      digitalWrite(LEDGREEN, HIGH);
      Serial.print("GREEN ");
    }
  } else {
    digitalWrite(LEDRED, LOW);
    digitalWrite(LEDGREEN, LOW);
  }
  Update7SEG();
}  // end of loop

void requestEvent() {

  Wire.write(IR * slave);      // if the ir is triggered send our slave number over ic2. If it is not we send a 0.
  Wire.write(BUTTON * slave);  // same as the IR but for the button
}


void recieveEvent() {
  LiveFloor = Wire.read();
  ChosenFloor = Wire.read();
  Serial.print("LiveFloor: ");
  Serial.println(LiveFloor);
  Serial.print("ChosenFloor: ");
  Serial.println(ChosenFloor);
  Serial.println();
}

void Update7SEG() {
  int i = LiveFloor;
    shiftOut(dataPin, clockPin, MSBFIRST, datArray[i + 1]);
}