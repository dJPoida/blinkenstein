#include <Arduino.h>
#include <Wire.h>
#include "servoController.h"
#include "config.h"

ServoController::ServoController(StateManager& stateManager) 
    : stateManager(stateManager), pwm(Adafruit_PWMServoDriver()), 
      servoPanPulse(0), servoTiltPulse(0), servoLeftLidTopPulse(0), 
      servoLeftLidBottomPulse(0), servoRightLidTopPulse(0), servoRightLidBottomPulse(0) 
{}

void ServoController::begin() {
    // Initialize I2C with specific SDA and SCL pins
    Wire.begin(PIN_SDA, PIN_SCL);

    pwm.begin();
    pwm.setPWMFreq(SERVO_PWM_FREQ);
}

void ServoController::update() {
    servoPanPulse = stateManager.getPanState();
    servoTiltPulse = stateManager.getTiltState();
    servoLeftLidTopPulse = stateManager.getLeftLidTopState();
    servoLeftLidBottomPulse = stateManager.getLeftLidBottomState();
    servoRightLidTopPulse = stateManager.getRightLidTopState();
    servoRightLidBottomPulse = stateManager.getRightLidBottomState();

    pwm.setPWM(SERVO_CHANNEL_PAN, 0, servoPanPulse);
    pwm.setPWM(SERVO_CHANNEL_TILT, 0, servoTiltPulse);
    pwm.setPWM(SERVO_CHANNEL_LEFT_LID_TOP, 0, servoLeftLidTopPulse);
    pwm.setPWM(SERVO_CHANNEL_LEFT_LID_BOTTOM, 0, servoLeftLidBottomPulse);
    pwm.setPWM(SERVO_CHANNEL_RIGHT_LID_TOP, 0, servoRightLidTopPulse);
    pwm.setPWM(SERVO_CHANNEL_RIGHT_LID_BOTTOM, 0, servoRightLidBottomPulse);
}

#ifdef SERIAL_DEBUG
void ServoController::printDebugValues() {
    char servoBuffer[256];
    snprintf(servoBuffer, sizeof(servoBuffer),
            "SERVOS: [PAN: %3d | TILT: %3d | LLT: %3d | LLB: %3d | RLT: %3d | RLB: %3d] ",
            servoPanPulse, servoTiltPulse, servoLeftLidTopPulse, servoLeftLidBottomPulse, servoRightLidTopPulse, servoRightLidBottomPulse);
    Serial.print(servoBuffer);
}
#endif