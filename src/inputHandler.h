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

    void update();

    int getJoystickXValue() const;
    int getJoystickXPercent() const;
    int getJoystickYValue() const;
    int getJoystickYPercent() const;
    int getPotPercent() const;
    int getPotValue() const;
    int getSmoothedPotValue() const;
    bool getButtonPressed() const;

    bool isManualControlEnabled() const;
    int getManualControlDisabledSinceMillis() const;

    bool isPowerButtonPressed();
    bool isPowerButtonDoublePressed();

    #ifdef SERIAL_DEBUG
    void printDebugValues();
    #endif

private:
    int joystickXValue;
    int joystickYValue;
    int potValue;
    int smoothedPotValue;
    bool buttonValue;

    bool manualControlEnabled;
    unsigned long timeSinceLastInput;
    unsigned long manualControlDisabledSinceMillis;
    unsigned long lastInputMillis;
    int lastAnalogInputChecksum;

    bool powerButtonState;
    unsigned long lastPowerButtonPressTime;
    bool powerButtonPressed;
    bool powerButtonDoublePressed;

    void readInputValues();
    void readPowerButton();
    int applyDeadzone(int value, int deadzone);
};

extern InputHandler inputHandler;

#endif