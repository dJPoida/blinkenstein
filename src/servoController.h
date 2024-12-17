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

class ServoController {
public:
    ServoController();

    void begin();
    void update(int panState, int tiltState, int topLidState, int bottomLidState);
    void checkI2CConnection();

    #ifdef SERIAL_DEBUG
    void printDebugValues();
    #endif

private:
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