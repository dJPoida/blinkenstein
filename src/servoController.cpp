/**
 * @file servoController.cpp
 * @brief Controls the servo motors for the Blinkenstein project.
 *
 * This class manages the initialization and updating of servo motors,
 * including setting their positions based on the current state.
 */

#include <Arduino.h>
#include <Wire.h>
#include "servoController.h"
#include "config.h"

/**
 * @brief Constructs a new ServoController object.
 */
ServoController::ServoController()
    : pwm(Adafruit_PWMServoDriver()),
      servoPanPulse(0), servoTiltPulse(0), servoLeftLidTopPulse(0),
      servoLeftLidBottomPulse(0), servoRightLidTopPulse(0), servoRightLidBottomPulse(0)
{}

/**
 * @brief Initializes the servo controller.
 */
void ServoController::begin() {
    // Initialize I2C with specific SDA and SCL pins
    Wire.begin(PIN_SDA, PIN_SCL);

    pwm.begin();
    pwm.setPWMFreq(SERVO_PWM_FREQ);
}

/**
 * @brief Updates the servo positions based on the current state.
 *
 * @param panState The pan state.
 * @param tiltState The tilt state.
 * @param topLidState The top lid state.
 * @param bottomLidState The bottom lid state.
 */
void ServoController::update(int panState, int tiltState, int topLidState, int bottomLidState) {
    checkI2CConnection();

    servoPanPulse = map(panState * -1, -100, 100, SERVO_PAN_MIN, SERVO_PAN_MAX);
    servoTiltPulse = map(tiltState * -1, -100, 100, SERVO_TILT_MIN, SERVO_TILT_MAX);

    servoLeftLidTopPulse = map(topLidState, 0, 100, SERVO_LEFT_LID_TOP_CLOSED, SERVO_LEFT_LID_TOP_OPEN);
    servoLeftLidBottomPulse = map(bottomLidState, 0, 100, SERVO_LEFT_LID_BOTTOM_CLOSED, SERVO_LEFT_LID_BOTTOM_OPEN);
    servoRightLidTopPulse = map(topLidState, 0, 100, SERVO_RIGHT_LID_TOP_CLOSED, SERVO_RIGHT_LID_TOP_OPEN);
    servoRightLidBottomPulse = map(bottomLidState, 0, 100, SERVO_RIGHT_LID_BOTTOM_CLOSED, SERVO_RIGHT_LID_BOTTOM_OPEN);

    pwm.setPWM(SERVO_CHANNEL_PAN, 0, servoPanPulse);
    pwm.setPWM(SERVO_CHANNEL_TILT, 0, servoTiltPulse);
    pwm.setPWM(SERVO_CHANNEL_LEFT_LID_TOP, 0, servoLeftLidTopPulse);
    pwm.setPWM(SERVO_CHANNEL_LEFT_LID_BOTTOM, 0, servoLeftLidBottomPulse);
    pwm.setPWM(SERVO_CHANNEL_RIGHT_LID_TOP, 0, servoRightLidTopPulse);
    pwm.setPWM(SERVO_CHANNEL_RIGHT_LID_BOTTOM, 0, servoRightLidBottomPulse);
}

/**
 * @brief Checks the I2C connection and reinitializes if necessary.
 */
void ServoController::checkI2CConnection()
{
    // Check I2C connection by reading a register from the PWM driver
    Wire.beginTransmission(SERVO_I2C_ADDRESS);
    Wire.write(0x00); // Read mode register 1
    if (Wire.endTransmission() != 0 || Wire.requestFrom(SERVO_I2C_ADDRESS, 1) != 1) {
        // Attempt to reinitialize I2C if the connection is lost
        Wire.begin(PIN_SDA, PIN_SCL);
        pwm.begin();
        pwm.setPWMFreq(SERVO_PWM_FREQ);
    }
}

#ifdef SERIAL_DEBUG
/**
 * @brief Prints the current servo values for debugging purposes.
 */
void ServoController::printDebugValues() {
    char servoBuffer[256];
    snprintf(servoBuffer, sizeof(servoBuffer),
            "SERVOS: [PAN: %3d | TILT: %3d | LLT: %3d | LLB: %3d | RLT: %3d | RLB: %3d] ",
            servoPanPulse, servoTiltPulse, servoLeftLidTopPulse, servoLeftLidBottomPulse, servoRightLidTopPulse, servoRightLidBottomPulse);
    Serial.print(servoBuffer);
}
#endif