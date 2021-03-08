
class Encoder {

  private:
    // Characterized quadrature patterns to detect a rotation of the motor shaft
    int m0States[4] = {0, 1, 1, 0};
    int m1States[4] = {0, 0, 1, 1};
    int numEncStates = 4;
    // TODO: deal with overflow, micros overflows after 70 minutes
    unsigned long lastQRotTime = micros();
    long lastQRot = 0;

  public:
    float gearRatio;
    int m0Pin;
    int m1Pin;
    int m0State;
    int m1State;
    int encState; // An index from 0 to 3 into the m0 and m1 state arrays
    int dir = 0;
    int numSkips = 0;
    long qRots = 0; // Number of quarter rotations done
    float RPM = 0;
    int RPMAvgCycles = 1; // Number of rotations for calculating local rpm

    Encoder(int encPin0, int encPin1, float inGearRatio) {
      gearRatio = inGearRatio;
      m0Pin = encPin0;
      m1Pin = encPin1;
      getState();

    }
    // Assuming our encoder will catch every state change
    void update() {
      int prevState = encState;
      getState();

      int rotNum = (encState + (numEncStates - prevState)) % numEncStates;
      switch (rotNum) {
        case 0 :
          break;
        case 1 :
          qRots += 1;
          dir = 1;
          break;
        case 3 :
          qRots -= 1;
          dir = -1;
          break;
        case 2 :
          numSkips += 1;
          if (dir == 1) {
            qRots += 2;
          }
          else if (dir == -1) {
            qRots -= 2;
          }
          break;
        default :
          Serial.println("Invalid encoder state! This should never happen!");
      }
      if (abs(qRots - lastQRot) >= RPMAvgCycles * numEncStates) {
        unsigned long dt = micros() - lastQRotTime;
        // There are 12 counts per revolution and we thought there were only four. That is why we
        // divide by three here.
        RPM = (RPMAvgCycles * 60 * pow(10 , 6)) / (dt * gearRatio * 3); // converting to RPM from microseconds
        lastQRot = qRots;
        lastQRotTime = micros();
      }
    }

    void getState() {
      /**
         Gets the current state of the encoder and updates the encState
         internal variable.
      */
      m0State = digitalRead(m0Pin);  // read the input pin
      m1State = digitalRead(m1Pin);

      for (int i = 0; i < 4; i++) {
        if ((m0State == m0States[i]) && (m1State == m1States[i])) {

          encState = i;
        }
      }
    }
};
