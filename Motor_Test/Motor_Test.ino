 class Motors {
    private Encoder trainEncoder = Encoder(ENCODER_PIN_A, ENCODER_PIN_B);
      int trainEncoderCount = 0;

    public:
      Servo trainMotor;
      #if ENABLE_DOORS
        Servo doorServo;
      #endif

      /*
        Ease train into provided speed setting:
          0 = No movement (stopped)
          1 = Slow speed
          2 = Normal speed
      */
      void easeTrainSpeed(int speed) {
        // TODO: Write this for ease
        ::state.trainSpeed = speed;
      }

      void setTrainSpeed(int speed) {
        switch (speed) {
          case 0:
            trainMotor.writeMicroseconds(1500);
            #if DEBUGf
              Serial.println(" - SET TRAIN SPEED: Stop");
            #endif
            break;

          case 1:
            trainMotor.writeMicroseconds(::state.trainDirection 
              ? 1500 + ((MOTOR_DRIVER_MAX - 1500) / 2) 
              : MOTOR_DRIVER_MIN + ((1500 - MOTOR_DRIVER_MIN) / 2));
            #if DEBUG
              Serial.println(" - SET TRAIN SPEED: Slow");
            #endif
            break;

          case 2:
            trainMotor.writeMicroseconds(::state.trainDirection 
              ? MOTOR_DRIVER_MAX 
              : MOTOR_DRIVER_MIN);
            #if DEBUG
              Serial.println(" - SET TRAIN SPEED: Normal");
            #endif
            break;
        }
        ::state.trainSpeed = speed;
      }
