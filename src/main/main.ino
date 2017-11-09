#include <Button.h>
#include <RotaryEncoder.h>

#define ROTARYSTEPS 1
#define ROTARYMIN 0

Button redButton(10);
Button encButton(17);   //pin A3 maps to 17

RotaryEncoder encoder(A1, A2);
int lastEncPos = -1;      //Last known rotary position.

int EncMode = 1;          //1=Menu, 2=Volume, 3=Frequency
int RotaryMaxSteps = 2;   // Maximum Steps (Var as it will change depending on Encoder Purpose)

int MenuSelection = 1;    //1=Fixed Mode, 2=SD Midi, 3=Live Midi

void setup() {
  redButton.begin();
  encButton.begin();
  
  while(!Serial);
  Serial.begin(57600);
  Serial.println("LimitedRotator example for the RotaryEncoder library.");
  
  encoder.setPosition(0); // start with the value of 0.
}

void loop() {
  int curEncPos = CurrentEncoderVal();
  
  if (lastEncPos != curEncPos) {
    Serial.println(curEncPos);
    lastEncPos = curEncPos;
  }

//  if(redButton.pressed()) {
//    
//  }

}


int CurrentEncoderVal() {
  encoder.tick();

  // get the current physical position and calc the logical position
  int newEncPos = encoder.getPosition() * ROTARYSTEPS;

  if (newEncPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newEncPos = ROTARYMIN;

  } else if (newEncPos > RotaryMaxSteps) {
    encoder.setPosition(RotaryMaxSteps / ROTARYSTEPS);
    newEncPos = RotaryMaxSteps;
  }

  return newEncPos;
}
