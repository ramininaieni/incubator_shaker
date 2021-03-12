
class Encoder {

  private:
    // Characterized quadrature patterns to detect a rotation of the motor shaft
    int m0States[4] = {0, 1, 1, 0};
    int m1States[4] = {0, 0, 1, 1};
    int numEncStates = 4;
    // TODO: deal with overflow, micros overflows after 70 minutes
    unsigned long lastQRotTime = micros();
    long lastQRot = 0;
    unsigned int encStatesPerRot;

  public:
    float gearRatio;
    int m0Pin;
    int m1Pin;
    int m0State;
    int m1State;
    int encState; // An index from 0 to 3 into the m0 and m1 state arrays
    int dir = 0;
    int numSkips = 0;
    long qCycles = 0; // Number of quarter cycles done
    float RPM = 0;
    int numCyclesToAvg = 1; // Number of cycles for calculating local rpm 
    float minRPM = 1;
    Encoder(int m0Pin, int m1Pin, float gearRatio, int encStatesPerRot): m0Pin ( m0Pin ), m1Pin ( m1Pin ),
    gearRatio ( gearRatio ), encStatesPerRot ( encStatesPerRot )
    {
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
          qCycles += 1;
          dir = 1;
          break;
        case 3 :
          qCycles -= 1;
          dir = -1;
          break;
        case 2 :
          numSkips += 1;
          if (dir == 1) {
            qCycles += 2;
          }
          else if (dir == -1) {
            qCycles -= 2;
          }
          break;
        default :
          Serial.println("Invalid encoder state! This should never happen!");
      }
      if (abs(qCycles - lastQRot) >= numCyclesToAvg * numEncStates) {
        unsigned long dt = micros() - lastQRotTime;
        float rotations = ((float)numEncStates/ encStatesPerRot)*(numCyclesToAvg)/gearRatio;
        float minutes = (dt/(pow(10 , 6)*60.0));
        RPM = rotations/minutes;
        lastQRot = qCycles;
        lastQRotTime = micros();
      }
      else if ((micros() - lastQRotTime) > 100000) {
        
        RPM = 0;
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
