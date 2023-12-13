#define SLAVE1 21
#define SLAVE2 22
#define SLAVE3 23
#define SLAVE4 24

#include <Wire.h>
#include <Keypad.h>

int Current_floor;
int Lfloor = 0;  //not sure if this one gets updated by the livefloor function
char Temp;
int MagicSequence[3][2];
double Start_time = millis();
float TTime;

int x = 0;
int y = 0;

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



#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12
int step_number = 0;



int Live_floor() {
  for(int i = 0; i < 3; i++){
  if (MagicSequence[i][1] == 1) { Lfloor = i + 1; }
}
  return (Lfloor);
}



void SEG_write(int Number) {}

void Go_to_floor(int Floor) {
  Floor;
  switch (Floor) {
    case 1:
      Wire.beginTransmission(21);
      Wire.write(1);
      Wire.end();

      Wire.beginTransmission(22);
      Wire.write(0);
      Wire.end();

      Wire.beginTransmission(23);
      Wire.write(0);
      Wire.end();
      break;
    case 2:
      Wire.beginTransmission(21);
      Wire.write(0);
      Wire.end();

      Wire.beginTransmission(22);
      Wire.write(1);
      Wire.end();

      Wire.beginTransmission(23);
      Wire.write(0);
      Wire.end();
      break;
    case 3:
      Wire.beginTransmission(21);
      Wire.write(0);
      Wire.end();

      Wire.beginTransmission(22);
      Wire.write(0);
      Wire.end();

      Wire.beginTransmission(23);
      Wire.write(1);
      Wire.end();
      break;
    default:
      break;
  }
  if (Floor < Live_floor())
    while (MagicSequence[1][Floor] != 1) {
      if (TTime > 2000){
     OneStep(false); //Stepmoter down on this line
      Start_time = millis();
      }
    }
  else if (Floor > Live_floor())
    while (MagicSequence[1][Floor] != 1) {
    if (TTime > 2000){
     OneStep(true); //Stepmoter up on this line
      Start_time = millis();
      }
    }
  else {}

  return;
}



void OneStep(bool dir){
    if(dir){
switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
}
  }else{
    switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
break;
}
  }
step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}





void setup() {
  Wire.begin();  // join i2c bus (address optional for master)
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  Serial.begin(9600);  // start serial for output
}


void loop() {
  TTime = millis() - Start_time;
  //fill a 2d array with the magic sequence
  //don't forget arrays are zerobased

  Wire.requestFrom(SLAVE1, 2);
  MagicSequence[0][0] = 1;            //Wire.read();
  MagicSequence[0][1] = Wire.read();  // IR 1
  delay(1);

  Wire.requestFrom(SLAVE2, 2);
  MagicSequence[1][0] = 2;            //Wire.read();
  MagicSequence[1][1] = Wire.read();  // IR 2
  delay(1);

  Wire.requestFrom(SLAVE3, 2);
  MagicSequence[2][0] = 3;            //Wire.read();
  MagicSequence[2][1] = Wire.read();  // IR 3
  delay(1);

  // Print the whole thing for Debug purposes
  for (int y = 0; y < 3; y++)
    ;
  {
    Serial.print(".");
    for (int x = 0; x < 2; x++)
      ;
    {
      Serial.print(MagicSequence[y][x]);
    }
    Serial.println();
  }



  Temp = customKeypad.getKey();

  switch (Temp) {
    default:
      // Serial.println("Error Called to floor: Invalid input");
      break;


    case 1:
      Go_to_floor(1);
      break;


    case 2:
      Go_to_floor(2);
      break;


    case 3:
      Go_to_floor(3);
      break;
  }

  SEG_write(Live_floor());

  if (Temp) {
    Serial.println(Temp);
  }
  // end of loop
}