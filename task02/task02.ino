#include <TaskManagerIO.h>

bool pin5State;
bool pin3State;
bool pin6State;
bool pin9State;
bool pin10State;
bool pin11State;



void togglePin(int pin, bool pinState) {
    digitalWrite(pin, pinState);
    pinState = !pinState;           
}

void setup() {
  taskManager.scheduleFixedRate(222, togglePin(11,pin11State) togglePin11);
  taskManager.scheduleFixedRate(222, togglePin(10,pin10State) togglePin10);
  taskManager.scheduleFixedRate(222, togglePin(9,pin9State) togglePin9);
  taskManager.scheduleFixedRate(222, togglePin(5,pin5State) togglePin5);
  taskManager.scheduleFixedRate(222, togglePin(6,pin6State) togglePin6);
  taskManager.scheduleFixedRate(222, togglePin(3,pin3State) togglePin3);
}

void loop() {
  taskManager.runLoop();
}
