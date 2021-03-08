#include "encoder.h"
// Assigning pins to the quadrature encoders for the motor
int m0 = A0;
int m1 = A1;
Encoder enc = Encoder(m0, m1, 100.0);

// Assigning digital pin 3 to the motor to control off/on state
int motorPin = 3;

// The encoder logic states in order when rotating one direction
// (reverse the order to rotate the other direction)

// Visualizing encoder values for debugging
int plotBuffer1[400];
int plotBuffer2[400];
int plotWriteIndex = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(motorPin, OUTPUT);

  analogWrite(motorPin, 100);

  Serial.begin(115200);

}

void loop() {
  // shows that gear ratio is wrong and they sent Max the wrong motor with a gear ratio of like 1200 instead of 100
  for (int i = 0; i < 1000; i ++) {
    enc.update();
    //      if (enc.qRots > 1200){
    //        analogWrite(motorPin, 0);
    //        delay(1000);
    //        enc.qRots = 0;
    //        analogWrite(motorPin, 50);
    //
    //      }
  }
  Serial.println(enc.RPM);

}
