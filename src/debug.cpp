/**
 * @file debug.cpp
 * @brief Provides debugging utilities for the application.
 *
 * This file contains declarations for debugging functions and variables
 * that are used to print debug information when SERIAL_DEBUG is enabled.
 */

#include "debug.h"
#include "config.h"
#include "servoController.h"
#include "inputHandler.h"

#ifdef SERIAL_DEBUG
unsigned long previousDebugMillis;

/**
 * @brief Prints the current debug values for the application.
 */
void printDebugValues() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousDebugMillis >= DEBUG_INTERVAL) {
        previousDebugMillis = currentMillis;

        #if (DEBUG_INPUT == 1)
        inputHandler.printDebugValues();
        #endif

        #if (DEBUG_STATE == 1)
        stateManager.printDebugValues();
        #endif

        #if (DEBUG_SERVOS == 1)
        servoController.printDebugValues();
        #endif

        Serial.println();
    }
}
#endif