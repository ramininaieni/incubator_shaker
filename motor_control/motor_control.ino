
int m0 = A0;
int m1 = A1;
int motorPin = 3;

// The encoder logic states in order when rotating one direction
// (reverse the order to rotate the other direction)


int plotBuffer1[400];
int plotBuffer2[400];
int plotWriteIndex = 0;

class Encoder {
  private:
    int m0States = {0, 1, 1, 0};
    int m1States = {0, 0, 1, 1};

    int newM0State; // Allocate placeholders for computing state differences
    int newM1State;

    int numEncStates = 4;

  public:

    int m0Pin;
    int m1Pin;

    int m0State;
    int m1State;
    int encState; // An index from 0 to 3 into the m0 and m1 state arrays


    long qRots; // Number of quarter rotations done

    void update() {
      newM0State = digitalRead(m0Pin);  // read the input pin
      newM1State = digitalRead(m1Pin);


    }


    void getState() {
      /**
       * Gets the current state of the encoder and updates the encState
       * internal variable.
       */
      newM0State = digitalRead(m0Pin);  // read the input pin
      newM1State = digitalRead(m1Pin);

      for (int i = 0; i < 4; i++) {
        if ((newM0State == m0States[i]) && (newM1State == m1States[i])) {

          encState = i;
        }
      }
    }

    int incrementEncoder(int prevM0, int prevM1, int newM0, int newM1) {

      for i
    }

  void setup() {
      // put your setup code here, to run once:

      pinMode(motorPin, OUTPUT);

      analogWrite(motorPin, 0);

      Serial.begin(115200);


    }

    void loop() {


      plotBuffer1[plotWriteIndex] = digitalRead(m0);  // read the input pin
      plotBuffer2[plotWriteIndex] = digitalRead(m1);
      if (plotWriteIndex >= 400) {
        for (int i = 0; i < 400; ++i) {
          Serial.print(2 * plotBuffer1[i]);        // debug value
          Serial.print(',');
          Serial.println(3 * plotBuffer2[i]);
          delay(1);
        }
        delay(1000);
        plotWriteIndex = 0;
      }
      plotWriteIndex += 1;
    }
