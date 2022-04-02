# define EN 8 // stepper motor enable , active low
# define X_DIR 5 // X -axis stepper motor direction control
# define Y_DIR 6 // y -axis stepper motor direction control
# define Z_DIR 7 // z axis stepper motor direction control
# define X_STP 2 // x -axis stepper control
# define Y_STP 3 // y -axis stepper control
# define Z_STP 4 // z -axis stepper control
# define SPEED_INTERVAL 20

#include <TaskManagerIO.h>

bool pinX_STPState = false;
bool pinZ_STPState = false;


void toggle_X_STP() {
    digitalWrite(X_STP, pinX_STPState);
    pinX_STPState = !pinX_STPState;
}

void toggle_Z_STP() {
    digitalWrite(Z_STP, pinZ_STPState);
    pinZ_STPState = !pinZ_STPState;
}

void setup() {
    pinMode (X_DIR, OUTPUT); pinMode (X_STP, OUTPUT);
    pinMode (Z_DIR, OUTPUT); pinMode (Z_STP, OUTPUT);
    pinMode (EN, OUTPUT);
    digitalWrite (EN, LOW);
    
    taskManager.scheduleFixedRate(SPEED_INTERVAL, toggle_X_STP, TIME_MICROS);
    taskManager.scheduleFixedRate(SPEED_INTERVAL, toggle_Z_STP, TIME_MICROS);
}

void loop() {
    taskManager.runLoop();
}
