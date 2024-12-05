#include "brain.h"
#include "config.h"

Brain::Brain(InputHandler& inputHandler):
    inputHandler(inputHandler),
    panState(0),
    tiltState(0),
    eyelidsState(50),
    blinkState(false),
    manualControlEnabled(MANUAL_CONTROL_ENABLED_DEFAULT),
    previousUpdateMillis(0),
    updateInterval(BRAIN_UPDATE_INTERVAL)
{}

void Brain::begin() {
    randomSeed(analogRead(0)); // Initialize random seed
}

void Brain::update() {
    manualControlEnabled = inputHandler.getTimeSinceLastInput() <= MANUAL_CONTROL_TIMEOUT;

    // Only update the brain if manual control is not enabled
    if (!manualControlEnabled) {
        bool prevBlinkState = blinkState;

        unsigned long currentMillis = millis();
        if (currentMillis - previousUpdateMillis >= updateInterval) {
            previousUpdateMillis = currentMillis;
            randomizeStates();
        }

        if (prevBlinkState != blinkState) {
            perviousBlinkMillis = currentMillis;
        }

        // Stop blinking if the blink state is true and the blink duration has passed
        if (blinkState && currentMillis - perviousBlinkMillis >= BLINK_DURATION) {
            blinkState = false;
        }
    }
}

void Brain::randomizeStates() {
    // Randomly change pan and tilt values to a major new look direction
    if (random(0, BRAIN_MAX_CHANCE) < BRAIN_CHANCE_OF_MAJOR_LOOK_CHANGE) {

        // Return to centre?
        if (panState != 0 || tiltState != 0 && random(0, BRAIN_MAX_CHANCE) < BRAIN_CHANCE_OF_LOOK_RETURN_CENTRE) {
            panState = 0;
            tiltState = 0;
        } else {
            panState = random(-100, 181);   // max 90% left or right
            tiltState = random(-100, 181);  // max 90% up or down
        }

        // Also blink?
        blinkState = blinkState || (random(0, BRAIN_MAX_CHANCE) < BRAIN_CHANCE_OF_BLINK_WHILE_LOOK);
    }

    // Randomly change pan and tilt values to a minor new look direction
    else if (random(0, BRAIN_MAX_CHANCE) < BRAIN_CHANCE_OF_MINOR_LOOK_CHANGE) {
        panState = constrain(panState + random(-10, 11), -100, 100);   // max 5% left or right
        tiltState = constrain(tiltState + random(-10, 11), -100, 100);  // max 5% up or down
    }

    else if (random(0, BRAIN_MAX_CHANCE) < BRAIN_CHANCE_OF_EYELID_CHANGE) {
        eyelidsState = random(0, 91); // max 90% open
    }

    else if (random(0, BRAIN_MAX_CHANCE) < BRAIN_CHANCE_OF_BLINK) {
        blinkState = 1;
    }
}


/**
 * @brief Checks if the brain is under manual control
 *
 * @return bool True if the brain is ignored by the state manager and manual input is overriding the brain
 */
bool Brain::isManualControlEnabled() const {
    return manualControlEnabled;
}

int Brain::getPanState() const {
    return panState;
}

int Brain::getTiltState() const {
    return tiltState;
}

int Brain::getEyelidsState() const {
    return eyelidsState;
}

bool Brain::getBlinkState() const {
    return blinkState;
}

/**
 * @brief Prints the current input values for debugging purposes.
 */
void Brain::printDebugValues() {
    char buffer[256];
    snprintf(buffer, sizeof(buffer),
            "BRAIN: [MAN CTL: %d | PAN: %4d | TILT: %4d | LIDS: %4d | BLINK: %d] ",
            manualControlEnabled, panState, tiltState, eyelidsState, blinkState);
    Serial.print(buffer);
}
