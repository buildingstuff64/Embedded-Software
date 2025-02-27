#include <stdint.h>

#define toggle(a) a = -a;

struct button_state_t{
  uint8_t pin;
  bool isOn;
  bool state;
};

//output pins
const uint8_t DATA = 1;
const uint8_t SYNC = 2;

//Pulsetrain pararmeters
const int a = 1000;
const int b = 1200;
const int c = 11;
const int d = 6500; 

//input pins
button_state_t OUTPUT_ENABLE;
button_state_t OUTPUT_SELECT;

const int32_t input_check_time = 1000;
int32_t last_check_time = 0;

void setup() {
  Serial.begin(9600);
  pinMode(DATA, OUTPUT);
  pinMode(SYNC, OUTPUT);

  //input state setup (pin, prevState, state)
  OUTPUT_ENABLE = {5, 0, 1};
  OUTPUT_SELECT = {6, 0, 0};

  pinMode(OUTPUT_ENABLE.pin, INPUT);
  pinMode(OUTPUT_SELECT.pin, INPUT);

}

void loop() {

  //check input pins before starting next pulse train 
  checkInputPin(&OUTPUT_ENABLE);
  checkInputPin(&OUTPUT_SELECT);

  if (OUTPUT_ENABLE.state){
    createPulseTrain();
  }

}

void checkInputPin(button_state_t *state){
    bool input = digitalRead(state->pin);
    if (input){
      state->state = state->state ? 0 : 1;
    }
    //pause input until button is unpressed to avoid infinite toggling / faulty buttons
    while(digitalRead(state->pin)){
      continue;
    }
}

void createPulseTrain(){
  createSyncPulse();
  for (int i=1; i <= getC(); i++){
    createPulse(getT(i), b);
  }
  digitalWrite(DATA, LOW);
  delayMicroseconds(d);
}

void createPulse(int onDuration, int offDuration){
    digitalWrite(DATA, HIGH);
    delayMicroseconds(onDuration);
    digitalWrite(DATA, LOW);
    delayMicroseconds(offDuration);
}

void createSyncPulse(){
  digitalWrite(SYNC, HIGH);
  delayMicroseconds(50);
  digitalWrite(SYNC, LOW);
}

int getT(int n){
  return a + ((n-1) * 50);
}

int getC(){
  return c - ((OUTPUT_SELECT.state) ? 3 : 0);
}
