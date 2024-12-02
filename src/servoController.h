/**
 * @file servoController.h
 * @brief Controls the servo motors for the Blinkenstein project.
 *
 * This class manages the initialization and updating of servo motors,
 * including setting their positions based on the current state.
 */

#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Adafruit_PWMServoDriver.h>
#include "config.h"
#include "stateManager.h"

class ServoController {
public:
    ServoController(StateManager& stateManager);

    void begin();
    void update();
    void checkI2CConnection();
    void printDebugValues();

private:
    StateManager& stateManager;

    Adafruit_PWMServoDriver pwm;

    int servoPanPulse;
    int servoTiltPulse;
    int servoLeftLidTopPulse;
    int servoLeftLidBottomPulse;
    int servoRightLidTopPulse;
    int servoRightLidBottomPulse;
};

extern ServoController servoController;

#endif