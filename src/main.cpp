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
ServoController servoController;
StateManager stateManager(inputHandler);

/**
 * @brief Setup function for the Blinkenstein control code.
 */
void setup() {
    #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    #endif

    // Initialize the PCA9685 board
    servoController.begin();

    // Initialize the state
    stateManager.begin();

    #ifdef SERIAL_DEBUG
    Serial.println("Setup complete. Starting loop...");
    #endif
}

/**
 * @brief Main loop for the Blinkenstein control code.
 */
void loop() {
    // Update input values (needs to be done outside the stateManager to enable power control)
    inputHandler.update();

    // Update the state manager
    stateManager.update();

    // Update the Servo Controller
    servoController.update(stateManager.getPanState(), stateManager.getTiltState(), stateManager.getTopLidState(), stateManager.getBottomLidState());

    // Output debug information
    #ifdef SERIAL_DEBUG
    printDebugValues();
    #endif
}
