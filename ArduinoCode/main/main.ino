#include <stdint.h>

#define GPIO_D0 16
#define GPIO_D1 5
#define GPIO_D2 4
#define GPIO_D3 0
#define GPIO_D4 2

const uint8_t DATA = 5;
const uint8_t SYNC = 4;

const uint8_t a = 1000;
const uint8_t b = 1200;
const uint8_t c = 11;
const uint8_t d = 6500; 

void setup() {
  Serial.begin(9600);
  pinMode(DATA, OUTPUT);
  pinMode(SYNC, OUTPUT);

}

void loop() {
  createPulseTrain();

}

void createPulseTrain(){
  Serial.println("starting new train");
  createSyncPulse();
    for (int i=1; i < c+1; i++){
      createPulse(getT(a), b);
    }
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
