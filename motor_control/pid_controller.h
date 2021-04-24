class PIDController {
//  private:

  public:
      float I_ = 0;
    float target_ = 0;
    float lastError_ = 0;
    unsigned long lastTime_ = micros();
    float kP;
    float kI;
    float kD;
  
    PIDController(float kP, float kI, float kD): kP ( kP ), kI ( kI ), kD ( kD )
    {
       
    }

    float update(float measurement) {
      float error = target_ - measurement;
      float dT = (micros() - lastTime_)/pow(10, 6);
      lastTime_ = micros();
      float dEdT = (error - lastError_)/dT;
      I_ += error*dT;
      return kP*error + kI*I_- kD*dEdT;
      
      
    }

    void setTarget(float target) {
      target_ = target;
    }
};
