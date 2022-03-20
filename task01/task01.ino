struct LightDiode {
  const int pin;
  int state;
  unsigned long prevMs;
  const unsigned long interval;
};

unsigned long getIntervalValue(int value) {
  return value * 1000;
}

#define diodesCount 5
struct LightDiode diodes[diodesCount] = {
  { 11, LOW, 0, getIntervalValue(10000) },
  { 10, LOW, 0, getIntervalValue(25000) },
  { 9, LOW, 0, getIntervalValue(7000) },
  { 6, LOW, 0, getIntervalValue(330) },
  { 5, LOW, 0, getIntervalValue(120) },
};
  
void setup() {
  for(int i = 0; i < diodesCount; i++) {
    pinMode(diodes[i].pin, OUTPUT);
  }
}

int tryBlink(struct LightDiode* diode, const unsigned long currentMs) {
  if (currentMs - diode->prevMs >= diode->interval) {
    diode->prevMs = currentMs;
    diode->state = (diode->state == LOW) ? HIGH : LOW;
    digitalWrite(diode->pin, diode->state);
    return 1;
  }
  return 0;
}

void loop() {
  unsigned long currentMs = micros();  
  for(int i = 0; i < diodesCount; i++) {
    tryBlink(&diodes[i], currentMs);
  }
}
