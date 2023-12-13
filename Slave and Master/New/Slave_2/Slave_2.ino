#include <Wire.h>

#define slave 1
#define IR 7
#define LEDGREEN 6
#define LEDRED 5

int LiveFloor = 0;
int ChosenFloor = 0;

void setup() {
  Wire.begin(slave);
  Serial.begin(9600);
  Wire.onRequest(requestEvent);
  Wire.onReceive(recieveEvent);
}

void loop() {
  if (digitalRead(IR) == 1) { // now this nested if statement toggles the leds on if the IR is triggered. If this floor is the Chosen Floor the green led will also turn on.
    digitalWrite(LEDRED, HIGH);
    if (slave == LiveFloor) {
      digitalWrite(LEDGREEN, HIGH);
    } else {
      digitalWrite(LEDGREEN, LOW);
    }
  } else {
    digitalWrite(LEDRED, LOW);
  }
}

void requestEvent() {
  Wire.write(digitalRead(IR) * slave);  // if the ir is triggered send our slave number over ic2. If it is not we send a 0.
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
