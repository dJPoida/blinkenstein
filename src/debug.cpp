#include "debug.h"
#include "servoController.h"
#include "inputHandler.h"

#ifdef SERIAL_DEBUG
unsigned long previousDebugMillis;

void printDebugValues() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousDebugMillis >= DEBUG_INTERVAL) {
        previousDebugMillis = currentMillis;

        #if (DEBUG_INPUT == 1)
        inputHandler.printDebugValues();
        #endif

        #if (DEBUG_STATE == 1)
        // TODO: Add state debug output if needed
        #endif

        #if (DEBUG_SERVOS == 1)
        servoController.printDebugValues();
        #endif

        Serial.println();
    }
}
#endif