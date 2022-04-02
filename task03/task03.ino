# define EN 8 // stepper motor enable , active low
# define X_DIR 5 // X -axis stepper motor direction control
# define Y_DIR 6 // y -axis stepper motor direction control
# define Z_DIR 7 // z axis stepper motor direction control
# define X_STP 2 // x -axis stepper control
# define Y_STP 3 // y -axis stepper control
# define Z_STP 4 // z -axis stepper control
# define SPEED_INTERVAL 20

struct Motor{
  bool dir;
  int stepperPin;
  unsigned long previousMicros;
  unsigned long interval; 
  int state;
};

struct Motor motors[] = {
  false, X_STP, 0, SPEED_INTERVAL, LOW,
  false, Z_STP, 0, SPEED_INTERVAL, LOW
};

void setup () {
  pinMode (Z_DIR, OUTPUT); pinMode (Z_STP, OUTPUT);
  pinMode (X_DIR, OUTPUT); pinMode (X_STP, OUTPUT);
  pinMode (EN, OUTPUT);
  digitalWrite (EN, LOW);
}

void loop () {
  
  unsigned long currentMicros = micros();
  digitalWrite (motors[0].stepperPin, false);
  digitalWrite (motors[1].stepperPin, false);
  
  for (int i = 0; i < 2; i++)
  {
    if (currentMicros - motors[i].previousMicros > motors[i].interval)
    {
      motors[i].previousMicros  = currentMicros;

      if (motors[i].state == LOW)
        motors[i].state = HIGH;
      else
        motors[i].state = LOW;

      digitalWrite(motors[i].stepperPin, motors[i].state);
    }
  }
    Serial.print('\n');
}
