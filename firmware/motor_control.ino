#include <Adafruit_PCT2075.h>

#include "encoder.h"
#include "pid_controller.h"

// Make PTC2075 Instance
Adafruit_PCT2075 PCT2075;

// Assigning pins to the quadrature encoders for the motor
int m0 = A0;
int m1 = A1;
Encoder enc = Encoder(m0, m1, 100.0, 12);
PIDController pid = PIDController(0, 5, 0);

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

  Serial.begin(115200);

  motorSetup();
  tempSetup();
  
}

void tempSetup(){
  if (!PCT2075.begin()) {
    Serial.println("Couldn't find PCT2075 chip");
    while (1);
  }
  Serial.println("Found PCT2075 chip");
}

void motorSetup(){
  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0);
  pid.setTarget(10);
  enc.numCyclesToAvg = 2;

}

void motorUpdate(){
  float res;
  int numAvg = 1000;
  float avgEnc = 0;
  for (int i = 0; i < numAvg; i ++) {
    enc.update();
    res = pid.update(enc.RPM);
    if (res > 255){
      res = 255;
    }
    if (res < 0) {
      res = 0;
    }
    analogWrite(motorPin, res);
    //      if (enc.qRots > 1200){
    //        delay(1000);
    //        enc.qRots = 0;
    //        analogWrite(motorPin, 50);
    //
    //      }
    avgEnc += enc.RPM;
  }
  avgEnc = avgEnc / numAvg;
  Serial.print(avgEnc);
  Serial.print(',');
  Serial.print(res);
  Serial.print(',');
  Serial.println(pid.I_);
}

void tempUpdate(){
  Serial.print("Temperature: "); Serial.print(PCT2075.getTemperature());Serial.println(" C");
  delay(100);
}

void loop() {
  
//  motorUpdate();
  tempUpdate();

}
