/**
 * @mainpage Blinkenstein - An Arduino program for controlling Ikkalebob's animatronic eyes
 * @author dJPoida
 * @see https://github.com/dJPoida/blinkenstein
 * @date 2024-11-18
 *
 * @file main.cpp
 * @brief Main setup and loop entry point for the Blinkenstein control code.
 */

#include <Arduino.h>
#include <SPI.h>

#include "config.h"
#include "inputHandler.h"
#include "servoController.h"
#include "stateManager.h"
#include "debug.h"

InputHandler inputHandler;
Brain brain(inputHandler);
StateManager stateManager(inputHandler, brain);
ServoController servoController(stateManager);

void setup() {
    #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    #endif

    // Initialize the PCA9685 board
    servoController.begin();

    // Initialize the brain
    brain.begin();

    #ifdef SERIAL_DEBUG
    Serial.println("Setup complete. Starting loop...");
    #endif
}

void loop() {
    // Update input values
    inputHandler.update();

    // Check for double power button press
    if (stateManager.getPowerState() && inputHandler.isPowerButtonDoublePressed()) {
        stateManager.setPowerState(false);
    } else if (!stateManager.getPowerState() && inputHandler.isPowerButtonPressed()) {
        stateManager.setPowerState(true);
    }

    if (stateManager.getPowerState()) {
        // Think
        brain.update();

        // Update state based on input values
        stateManager.update();

        // Update servos based on state changes
        servoController.update();
    }

    // Output debug information
    #ifdef SERIAL_DEBUG
    printDebugValues();
    #endif
}
