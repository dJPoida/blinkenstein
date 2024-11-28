#include "StateManager.h"
#include "config.h"
#include <Arduino.h>

StateManager::StateManager() 
    : panState(0), tiltState(0), leftLidTopState(0), leftLidBottomState(0), rightLidTopState(0), rightLidBottomState(0) 
{}

void StateManager::update(int joystickXPercent, int joystickYPercent, int potPercent, bool buttonPressed) {
    panState = map(joystickXPercent * -1, -100, 100, SERVO_PAN_MIN, SERVO_PAN_MAX);
    tiltState = map(joystickYPercent * -1, -100, 100, SERVO_TILT_MIN, SERVO_TILT_MAX);

    int newEyeLidsClosedAmount = buttonPressed ? 100 : potPercent;

    leftLidTopState = map(newEyeLidsClosedAmount, 0, 100, SERVO_LEFT_LID_TOP_OPEN, SERVO_LEFT_LID_TOP_CLOSED);
    leftLidBottomState = map(newEyeLidsClosedAmount, 0, 100, SERVO_LEFT_LID_BOTTOM_OPEN, SERVO_LEFT_LID_BOTTOM_CLOSED);
    rightLidTopState = map(newEyeLidsClosedAmount, 0, 100, SERVO_RIGHT_LID_TOP_OPEN, SERVO_RIGHT_LID_TOP_CLOSED);
    rightLidBottomState = map(newEyeLidsClosedAmount, 0, 100, SERVO_RIGHT_LID_BOTTOM_OPEN, SERVO_RIGHT_LID_BOTTOM_CLOSED);
}

int StateManager::getPanState() const {
    return panState;
}

int StateManager::getTiltState() const {
    return tiltState;
}

int StateManager::getLeftLidTopState() const {
    return leftLidTopState;
}

int StateManager::getLeftLidBottomState() const {
    return leftLidBottomState;
}

int StateManager::getRightLidTopState() const {
    return rightLidTopState;
}

int StateManager::getRightLidBottomState() const {
    return rightLidBottomState;
}