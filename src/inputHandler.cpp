/**
 * @file inputHandler.cpp
 * @brief Handles input from various hardware components such as joysticks, potentiometers, and buttons.
 *
 * This class reads input values from hardware components, processes them, and provides methods to access
 * the current state of these inputs.
 */

#include <Arduino.h>
#include "inputHandler.h"

InputHandler::InputHandler():
    joystickXValue(0),
    joystickYValue(0),
    potValue(0),
    buttonValue(false),
    prevJoystickXValue(0),
    prevJoystickYValue(0),
    prevPotValue(0),
    prevButtonValue(false),
    smoothedPotValue(0),
    timeSinceLastInput(MANUAL_CONTROL_ENABLED_DEFAULT ? MANUAL_CONTROL_TIMEOUT + 1 : 0),
    lastInputMillis(0),
    lastAnalogInputChecksum(0),
    powerButtonState(false),
    lastPowerButtonPressTime(0),
    powerButtonDoublePressed(false)
{
    // This pin is attached tothe CKCS module K pin which when grounded will power off the charge module.
    // However, when charging, the CKCS module will not power off the ESP32
    // If the operater presses this button twice, we will use software to stop the code from running
    // Under normal battery operated conditions, the CKCS module will power off the ESP32
    pinMode(PIN_POWER_BUTTON, INPUT_PULLUP);

    pinMode(PIN_BLINK_BUTTON, INPUT_PULLUP);
    pinMode(PIN_BLINK_BUTTON_2, INPUT_PULLUP);
    pinMode(PIN_JOYSTICK_X, INPUT);
    pinMode(PIN_JOYSTICK_Y, INPUT);
    pinMode(PIN_EYELIDS_POT, INPUT);
}

/**
 * @brief Update the input values and return true if any input has changed.
 *
 * @return true if any input has changed, false otherwise.
 */
bool InputHandler::update() {
    readInputValues();

    return isJoystickXChanged() || isJoystickYChanged() || isPotValueChanged() || isButtonValueChanged();
}

/**
 * @brief Checks if the joystick X value has changed.
 *
 * @return true if the joystick X value has changed, false otherwise.
 */
bool InputHandler::isJoystickXChanged() const {
    return joystickXValue != prevJoystickXValue;
}

/**
 * @brief Checks if the joystick Y value has changed.
 *
 * @return true if the joystick Y value has changed, false otherwise.
 */
bool InputHandler::isJoystickYChanged() const {
    return joystickYValue != prevJoystickYValue;
}

/**
 * @brief Checks if the potentiometer value has changed.
 *
 * @return true if the potentiometer value has changed, false otherwise.
 */
bool InputHandler::isPotValueChanged() const {
    return potValue != prevPotValue;
}

/**
 * @brief Checks if the button value has changed.
 *
 * @return true if the button value has changed, false otherwise.
 */
bool InputHandler::isButtonValueChanged() const {
    return buttonValue != prevButtonValue;
}

/**
 * @brief Gets the current joystick X value.
 *
 * @return the current joystick X value.
 */
int InputHandler::getJoystickXValue() const {
    return joystickXValue;
}

/**
 * @brief Gets the current joystick X value as a percentage.
 *
 * @return the current joystick X value as a percentage.
 */
int InputHandler::getJoystickXPercent() const {
    return map(joystickXValue, 0, 4095, -100, 100);
}

/**
 * @brief Gets the current joystick Y value.
 *
 * @return the current joystick Y value.
 */
int InputHandler::getJoystickYValue() const {
    return joystickYValue;
}

/**
 * @brief Gets the current joystick Y value as a percentage.
 *
 * @return the current joystick Y value as a percentage.
 */
int InputHandler::getJoystickYPercent() const {
    return map(joystickYValue, 0, 4095, -100, 100);
}

/**
 * @brief Gets the current potentiometer value.
 *
 * @return the current potentiometer value.
 */
int InputHandler::getPotValue() const {
    return potValue;
}

/**
 * @brief Gets the current potentiometer value as a percentage.
 *
 * @return the current potentiometer value as a percentage.
 */
int InputHandler::getPotPercent() const {
    return map(potValue, 0, 4095, 0, 100);
}

/**
 * @brief Gets the current button pressed state.
 *
 * @return true if the button is pressed, false otherwise.
 */
bool InputHandler::getButtonPressed() const {
    return buttonValue;
}

/**
 * @brief Gets the smoothed potentiometer value.
 *
 * @return the smoothed potentiometer value.
 */
int InputHandler::getSmoothedPotValue() const {
    return smoothedPotValue;
}

/**
 * @brief Reads the input values from the hardware and updates the internal state.
 */
void InputHandler::readInputValues() {
    prevJoystickXValue = joystickXValue;
    prevJoystickYValue = joystickYValue;
    prevPotValue = potValue;
    prevButtonValue = buttonValue;

    // Read the Joystick Values
    int newJoystickXValue = constrain(analogRead(PIN_JOYSTICK_X) + JOYSTICK_DRIFT_ADUSTMENT_X, 0, 4095);
    int newJoystickYValue = constrain(analogRead(PIN_JOYSTICK_Y) + JOYSTICK_DRIFT_ADUSTMENT_Y, 0, 4095);
    // Apply deadzone
    newJoystickXValue = applyDeadzone(newJoystickXValue, JOYSTICK_DEADZONE);
    newJoystickYValue = applyDeadzone(newJoystickYValue, JOYSTICK_DEADZONE);

    // Reat the Potentiometer Value
    int newPotValue = 4095 - constrain(analogRead(PIN_EYELIDS_POT), 0, 4095);
    int previousSmoothedPotValue = smoothedPotValue;
    smoothedPotValue = (SMOOTHING_FACTOR * newPotValue) + ((1 - SMOOTHING_FACTOR) * smoothedPotValue);

    // Read the Button Value
    int newButtonValue = !digitalRead(PIN_BLINK_BUTTON) || !digitalRead(PIN_BLINK_BUTTON_2);

    // Read the Power Button Value
    bool newPowerButtonState = !digitalRead(PIN_POWER_BUTTON);
    if (newPowerButtonState && !powerButtonState) {
        powerButtonPressed = true;

        unsigned long currentTime = millis();
        if (currentTime - lastPowerButtonPressTime >= 100 && currentTime - lastPowerButtonPressTime <= 500) {
            powerButtonDoublePressed = true;
            powerButtonPressed = false;
        }
        lastPowerButtonPressTime = currentTime;
    }
    powerButtonState = newPowerButtonState;

    // Determine whether the user is manually controlling the input
    if ((newButtonValue != buttonValue) ||
        (abs(newJoystickXValue + newJoystickYValue + newPotValue - lastAnalogInputChecksum) > MANUAL_CONTROL_INTERRUPT_THRESHOLD)
    ) {
        lastAnalogInputChecksum = newJoystickXValue + newJoystickYValue + newPotValue;
        lastInputMillis = millis();
    }
    timeSinceLastInput = millis() - lastInputMillis;

    // Update the input values
    joystickXValue = newJoystickXValue;
    joystickYValue = newJoystickYValue;
    potValue = newPotValue;
    buttonValue = newButtonValue;
}

/**
 * @brief Applies a deadzone to the joystick value.
 *
 * @param value The joystick value.
 * @param deadzone The deadzone threshold.
 * @return The joystick value with the deadzone applied.
 */
int InputHandler::applyDeadzone(int value, int deadzone) {
    if (abs(value - 2048) < deadzone) {
        return 2048;
    }
    return value;
}

/**
 * @brief Calculates the time since the last meaningful input.
 *
 * @return unsigned long The time in milliseconds since the last meaningful input.
 */
unsigned long InputHandler::getTimeSinceLastInput() const {
    // When manual control is not enabled by default, make sure the time since last input exceeds the timeout.
    // millis() starts at 0 which yields a false positive
    return (millis() < MANUAL_CONTROL_TIMEOUT) && !MANUAL_CONTROL_ENABLED_DEFAULT ? MANUAL_CONTROL_TIMEOUT + 1 : timeSinceLastInput;
}

/**
 * @brief Checks if the power button is pressed.
 *
 * @return true if the power button is pressed, false otherwise.
 */
bool InputHandler::isPowerButtonPressed() {
    if (powerButtonPressed) {
        powerButtonPressed = false;
        return true;
    }
    return false;
}

/**
 * @brief Checks if the power button is double pressed.
 *
 * @return true if the power button is double pressed, false otherwise.
 */
bool InputHandler::isPowerButtonDoublePressed() {
    if (powerButtonDoublePressed) {
        powerButtonDoublePressed = false;
        return true;
    }
    return false;
}

/**
 * @brief Prints the current input values for debugging purposes.
 */
void InputHandler::printDebugValues() {
    char inputBuffer[256];
    snprintf(inputBuffer, sizeof(inputBuffer),
            "INPUT: [JOY_X: %4d | JOY_Y: %4d | POT: %4d | BUTTON: %d | PWR: %d | PWR2: %d | TSLI : %6d] ",
            joystickXValue, joystickYValue, potValue, buttonValue, powerButtonPressed, powerButtonDoublePressed, timeSinceLastInput);
    Serial.print(inputBuffer);
}
