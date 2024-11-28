#include "inputHandler.h"


InputHandler::InputHandler(StateManager& stateManager):
    stateManager(stateManager),
    joystickXValue(0),
    joystickYValue(0),
    potValue(0),
    buttonValue(false),
    prevJoystickXValue(0),
    prevJoystickYValue(0),
    prevPotValue(0),
    prevButtonValue(false),
    smoothedPotValue(0)
{
    pinMode(PIN_BLINK_BUTTON, INPUT_PULLUP);
    pinMode(PIN_BLINK_BUTTON_2, INPUT_PULLUP);
    pinMode(PIN_JOYSTICK_X, INPUT);
    pinMode(PIN_JOYSTICK_Y, INPUT);
    pinMode(PIN_EYELIDS_POT, INPUT);
}

/**
 * Updates the input values and notifies the state manager if any input has changed.
 */
void InputHandler::update() {
    readInputValues();
    if (isJoystickXChanged() || isJoystickYChanged() || isPotValueChanged() || isButtonValueChanged()) {
        stateManager.update(getJoystickXPercent(), getJoystickYPercent(), getPotPercent(), getButtonPressed());
    }
}

/**
 * Checks if the joystick X value has changed.
 * @return true if the joystick X value has changed, false otherwise.
 */
bool InputHandler::isJoystickXChanged() const {
    return joystickXValue != prevJoystickXValue;
}

/**
 * Checks if the joystick Y value has changed.
 * @return true if the joystick Y value has changed, false otherwise.
 */
bool InputHandler::isJoystickYChanged() const {
    return joystickYValue != prevJoystickYValue;
}

/**
 * Checks if the potentiometer value has changed.
 * @return true if the potentiometer value has changed, false otherwise.
 */
bool InputHandler::isPotValueChanged() const {
    return potValue != prevPotValue;
}

/**
 * Checks if the button value has changed.
 * @return true if the button value has changed, false otherwise.
 */
bool InputHandler::isButtonValueChanged() const {
    return buttonValue != prevButtonValue;
}

/**
 * Gets the current joystick X value.
 * @return the current joystick X value.
 */
int InputHandler::getJoystickXValue() const {
    return joystickXValue;
}

/**
 * Gets the current joystick X value as a percentage.
 * @return the current joystick X value as a percentage.
 */
int InputHandler::getJoystickXPercent() const {
    return map(joystickXValue, 0, 4095, -100, 100);
}

/**
 * Gets the current joystick Y value.
 * @return the current joystick Y value.
 */
int InputHandler::getJoystickYValue() const {
    return joystickYValue;
}

/**
 * Gets the current joystick Y value as a percentage.
 * @return the current joystick Y value as a percentage.
 */
int InputHandler::getJoystickYPercent() const {
    return map(joystickYValue, 0, 4095, -100, 100);
}

/**
 * Gets the current potentiometer value.
 * @return the current potentiometer value.
 */
int InputHandler::getPotValue() const {
    return potValue;
}

/**
 * Gets the current potentiometer value as a percentage.
 * @return the current potentiometer value as a percentage.
 */
int InputHandler::getPotPercent() const {
    return map(potValue, 0, 4095, 0, 100);
}

/**
 * Gets the current button pressed state.
 * @return true if the button is pressed, false otherwise.
 */
bool InputHandler::getButtonPressed() const {
    return buttonValue;
}

/**
 * Gets the smoothed potentiometer value.
 * @return the smoothed potentiometer value.
 */
int InputHandler::getSmoothedPotValue() const {
    return smoothedPotValue;
}

/**
 * Reads the input values from the hardware and updates the internal state.
 */
void InputHandler::readInputValues() {
    prevJoystickXValue = joystickXValue;
    prevJoystickYValue = joystickYValue;
    prevPotValue = potValue;
    prevButtonValue = buttonValue;

    int newJoystickXValue = constrain(analogRead(PIN_JOYSTICK_X) + JOYSTICK_DRIFT_ADUSTMENT_X, 0, 4095);
    int newJoystickYValue = constrain(analogRead(PIN_JOYSTICK_Y) + JOYSTICK_DRIFT_ADUSTMENT_Y, 0, 4095);
    // Apply deadzone
    newJoystickXValue = applyDeadzone(newJoystickXValue, JOYSTICK_DEADZONE);
    newJoystickYValue = applyDeadzone(newJoystickYValue, JOYSTICK_DEADZONE);

    int newPotValue = 4095 - constrain(analogRead(PIN_EYELIDS_POT), 0, 4095);
    int previousSmoothedPotValue = smoothedPotValue;
    smoothedPotValue = (SMOOTHING_FACTOR * newPotValue) + ((1 - SMOOTHING_FACTOR) * smoothedPotValue);

    int newButtonValue = digitalRead(PIN_BLINK_BUTTON);
    int newButtonValue2 = digitalRead(PIN_BLINK_BUTTON_2);

    joystickXValue = newJoystickXValue;
    joystickYValue = newJoystickYValue;
    potValue = newPotValue;
    buttonValue = !(newButtonValue || newButtonValue2);
}

/**
 * Applies a deadzone to the joystick value.
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
 * Prints the current input values for debugging purposes.
 */
void InputHandler::printDebugValues() {
    char inputBuffer[256];
    snprintf(inputBuffer, sizeof(inputBuffer),
            "INPUT: [JOY_X: %4d | JOY_Y: %4d | POT: %4d | BUTTON: %d] ",
            joystickXValue, joystickYValue, potValue, buttonValue);
    Serial.print(inputBuffer);
}
