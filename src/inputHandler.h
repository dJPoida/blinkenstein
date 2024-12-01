/**
 * @file inputHandler.h
 * @brief Handles input from various hardware components such as joysticks, potentiometers, and buttons.
 *
 * This class reads input values from hardware components, processes them, and provides methods to access
 * the current state of these inputs.
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>
#include "config.h"

class InputHandler {
public:
    InputHandler();

    bool update();

    bool isJoystickXChanged() const;
    bool isJoystickYChanged() const;
    bool isPotValueChanged() const;
    bool isButtonValueChanged() const;

    int getJoystickXValue() const;
    int getJoystickXPercent() const;
    int getJoystickYValue() const;
    int getJoystickYPercent() const;

    int getPotPercent() const;
    int getPotValue() const;

    bool getButtonPressed() const;
    int getSmoothedPotValue() const;

    void printDebugValues();

private:
    int joystickXValue;
    int joystickYValue;
    int potValue;
    bool buttonValue;

    int prevJoystickXValue;
    int prevJoystickYValue;
    int prevPotValue;
    int smoothedPotValue;
    bool prevButtonValue;

    void readInputValues();
    int applyDeadzone(int value, int deadzone);
};

extern InputHandler inputHandler;

#endif