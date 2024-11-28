#include <Arduino.h>
#include <SPI.h>

#include "config.h"
#include "inputHandler.h"
#include "servoController.h"
#include "stateManager.h"
#include "debug.h"

StateManager stateManager;
InputHandler inputHandler(stateManager);
ServoController servoController(stateManager);

void setup() {
    #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    #endif

    // Initialize the PCA9685 board
    servoController.begin();

    #ifdef SERIAL_DEBUG
    Serial.println("Setup complete. Starting loop...");
    #endif
}

void loop() {
    // Update input values
    inputHandler.update();

    // Update servos based on state changes
    servoController.update();

    // Output debug information
    #ifdef SERIAL_DEBUG
    printDebugValues();
    #endif
}
