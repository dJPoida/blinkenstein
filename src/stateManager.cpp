/**
 * @file stateManager.h
 * @brief Manages the state of the Blinkenstein project.
 *
 * This class is responsible for maintaining and updating the state of various components,
 * such as the positions of servos and the state of inputs. It provides methods to access
 * the current state and update it based on input values.
 */

#include <Arduino.h>
#include "stateManager.h"
#include "config.h"

/**
 * @brief Constructs a new StateManager object.
 *
 * @param inputHandler Reference to the InputHandler object.
 */
StateManager::StateManager(InputHandler& inputHandler):
    inputHandler(inputHandler),
    powerState(true),
    panState(0),
    tiltState(0),
    panTwitchOffset(0),
    tiltTwitchOffset(0),
    topLidState(0),
    bottomLidState(0),
    autoBlinkState(false),
    autoEyelidsState(50),
    sleeping(false),
    previousAutoUpdateMillis(0),
    perviousAutoBlinkMillis(0)
{}

/**
 * @brief Initialize the state manager.
 */
void StateManager::begin() {
    randomSeed(analogRead(0)); // Initialize random seed
}

/**
 * @brief Updates the state based on the current input values or autonomous control.
 */
void StateManager::update() {
    // Ensure the lids are closed
    if (sleeping) {
        autoEyelidsState = 0;
        topLidState = 0;
        bottomLidState = 0;
    }

    // Don't continue if the bot is powered off (or soft powered off when charging)
    if (!checkPowerState()) {
        return;
    }

    int newPanState = panState;
    int newTiltState = tiltState;
    int newTopLidState = topLidState;
    int newBottomLidState = bottomLidState;
    int newAutoEyelidsState = autoEyelidsState;
    bool newAutoBlinkState = autoBlinkState;

    // Update the state based on the input values when under manual control
    if (inputHandler.isManualControlEnabled()) {
        // Bot can't be asleep if the manual control is enabled
        sleeping = false;

        int joystickXPercent = inputHandler.getJoystickXPercent();
        int joystickYPercent = inputHandler.getJoystickYPercent();
        int potPercent = inputHandler.getPotPercent();
        bool buttonPressed = inputHandler.getButtonPressed();

        newPanState = joystickXPercent;
        newTiltState = joystickYPercent;

        // Set the initial lid state based on the blink button or the potentiometer
        newTopLidState = buttonPressed ? 0 : potPercent;
        newBottomLidState = buttonPressed ? 0 : potPercent;

        // Set the auto eyelids state based on the potentiometer in anticipation of switching to autonomous control
        autoEyelidsState = potPercent;
    }

    // Update the state using autonomous control
    else {
        // Don't update if the bot is asleep
        if (!sleeping && powerState) {
            bool prevAutoBlinkState = autoBlinkState;

            unsigned long currentMillis = millis();
            if (currentMillis - previousAutoUpdateMillis >= AUTO_UPDATE_INTERVAL) {
                previousAutoUpdateMillis = currentMillis;
                randomizeStates(newPanState, newTiltState, newTopLidState, newBottomLidState, newAutoEyelidsState, newAutoBlinkState);
            }

            // Keep track of when the blink state changes
            if (prevAutoBlinkState != autoBlinkState) {
                perviousAutoBlinkMillis = currentMillis;
            }

            // Stop blinking if the blink state is true and the blink duration has passed
            if (newAutoBlinkState && currentMillis - perviousAutoBlinkMillis >= AUTO_BLINK_DURATION) {
                newAutoBlinkState = false;
            }

            if (millis() - inputHandler.getManualControlDisabledSinceMillis() >= (AUTO_POWER_OFF_TIMEOUT - 1000)) {
                #ifdef SERIAL_DEBUG
                Serial.println("Sleeping. Bot will power down in 1 second...");
                #endif

                // Sleep 1 second before power down
                sleeping = true;
                newAutoBlinkState = true;
            }
        } else if (sleeping && powerState && (millis() - inputHandler.getManualControlDisabledSinceMillis() >= AUTO_POWER_OFF_TIMEOUT)) {
            // power down if the bot has been inactive for a while
            powerDown();
        }
    }

    // Randomise a twitch on the eyeballs to emulate realism
    // The twitch is only applied to the servo positions, not the state
    if (random(0, AUTO_MAX_CHANCE) < AUTO_CHANCE_OF_LOOK_TWITCH) {
        panTwitchOffset = constrain(random(-AUTO_LOOK_TWITCH_AMOUNT, AUTO_LOOK_TWITCH_AMOUNT + 1), -100, 100);
        tiltTwitchOffset = constrain(random(-AUTO_LOOK_TWITCH_AMOUNT, AUTO_LOOK_TWITCH_AMOUNT + 1), -100, 100);
    }

    // Adjust the top and bottom lid states based on the tilt state so that the pupil is always visible
    if (newTiltState < 0 and newTopLidState > 0 and newTopLidState < PUPIL_REVEAL_LID_MAX_AMOUNT) {
        int offsetTopLidState = map(-newTiltState, 0, 100, PUPIL_REVEAL_LID_MIN_AMOUNT, PUPIL_REVEAL_LID_MAX_AMOUNT) - PUPIL_REVEAL_LID_MIN_AMOUNT;
        newTopLidState = constrain(newTopLidState + offsetTopLidState, 0, 100);
    }
    if (newTiltState > 0 and newBottomLidState > 0 and newBottomLidState < PUPIL_REVEAL_LID_MAX_AMOUNT) {
        int offsetBottomLidState = map(-newTiltState, 0, 100, PUPIL_REVEAL_LID_MIN_AMOUNT, PUPIL_REVEAL_LID_MAX_AMOUNT) - PUPIL_REVEAL_LID_MIN_AMOUNT;
        newBottomLidState = constrain(newBottomLidState - offsetBottomLidState, 0, 100);
    }

    // Update the state
    panState = newPanState;
    tiltState = newTiltState;
    topLidState = newTopLidState;
    bottomLidState = newBottomLidState;
    autoEyelidsState = newAutoEyelidsState;
    autoBlinkState = newAutoBlinkState;
}

/**
 * @brief Randomly changes the state of the bot when under autonomous control.
 */
void StateManager::randomizeStates(int& newPanState, int& newTiltState, int& newTopLidState, int& newBottomLidState, int& newAutoEyelidsState, bool& newAutoBlinkState) {
    // Randomly change pan and tilt values to a major new look direction
    if (random(0, AUTO_MAX_CHANCE) < AUTO_CHANCE_OF_MAJOR_LOOK_CHANGE) {
        // Return to centre?
        if (newPanState != 0 || newTiltState != 0 && random(0, AUTO_MAX_CHANCE) < AUTO_CHANCE_OF_LOOK_RETURN_CENTRE) {
            newPanState = 0;
            newTiltState = 0;
        } else {
            // choose a random new look direction based on the available positions
            newPanState = AUTO_LOOK_PAN_POSITIONS[random(0, AUTO_LOOK_PAN_POSITION_COUNT)];
            newTiltState = AUTO_LOOK_TILT_POSITIONS[random(0, AUTO_LOOK_TILT_POSITION_COUNT)];
        }

        // Also blink?
        newAutoBlinkState = newAutoBlinkState || (random(0, AUTO_MAX_CHANCE) < AUTO_CHANCE_OF_BLINK_WHILE_LOOK);

        #ifdef SERIAL_DEBUG
        Serial.println("RAND: Look: P" + String(newPanState) + ", T" + String(newTiltState) + String(newAutoBlinkState ? " (and blink)" : ""));
        #endif
    }

    if (random(0, AUTO_MAX_CHANCE) < AUTO_CHANCE_OF_EYELID_CHANGE) {
        newAutoEyelidsState = AUTO_SQUINT_POSITIONS[random(0, AUTO_SQUINT_POSITION_COUNT)];

        #ifdef SERIAL_DEBUG
        Serial.println("RAND: Squint: " + String(newAutoEyelidsState));
        #endif
    }

    if (random(0, AUTO_MAX_CHANCE) < AUTO_CHANCE_OF_BLINK) {
        newAutoBlinkState = 1;

        #ifdef SERIAL_DEBUG
        Serial.println("RAND: Blink");
        #endif
    }

    newTopLidState = newAutoBlinkState ? 0 : newAutoEyelidsState;
    newBottomLidState = newAutoBlinkState ? 0 : newAutoEyelidsState;
}

/**
 * @brief Checks the power state and toggles it if the power button is pressed.
 */
bool StateManager::checkPowerState() {
        if (powerState && inputHandler.isPowerButtonDoublePressed()) {
        setPowerState(false);
    } else if (!powerState && inputHandler.isPowerButtonPressed()) {
        setPowerState(true);
    }

    return powerState;
}

/**
 * @brief Toggle the power button pin from an input to an output and send a double pulse to the power button
 */
void StateManager::powerDown() {
    #ifdef SERIAL_DEBUG
    Serial.println("Sending powering down signal...");
    #endif

    powerState = false;

    // Set the power button pin to an output
    pinMode(PIN_POWER_BUTTON, OUTPUT);
    digitalWrite(PIN_POWER_BUTTON, HIGH);

    // If the bot is on battery power, the CKCS module will power off the ESP32
    delay(50);
    digitalWrite(PIN_POWER_BUTTON, LOW);
    delay(100);
    digitalWrite(PIN_POWER_BUTTON, HIGH);
    delay(100);
    digitalWrite(PIN_POWER_BUTTON, LOW);
    delay(100);
    digitalWrite(PIN_POWER_BUTTON, HIGH);
    delay(50);

    // Set the power button pin to an input again
    // If the bot is on battery power, this line may not be reached
    pinMode(PIN_POWER_BUTTON, INPUT_PULLUP);

    #ifdef SERIAL_DEBUG
    Serial.println("Power should be off by now");
    #endif
}

/**
 * @brief Sets the power state.
 *
 * @param state The new power state.
 */
void StateManager::setPowerState(bool state) {
    powerState = state;
}

/**
 * @brief Gets the current power state.
 *
 * @return int The current power state.
 */
bool StateManager::getPowerState() const {
    return powerState;
}

/**
 * @brief Gets the current pan state including the twitch offset.
 *
 * @return int The current pan state.
 */
int StateManager::getPanState() const {
    return panState + panTwitchOffset;
}

/**
 * @brief Gets the current tilt state including the twitch offset.
 *
 * @return int The current tilt state.
 */
int StateManager::getTiltState() const {
    return tiltState + tiltTwitchOffset;
}

/**
 * @brief Gets the current top lid state.
 *
 * @return int The current top lid state.
 */
int StateManager::getTopLidState() const {
    return topLidState;
}

/**
 * @brief Gets the current bottom lid state.
 *
 * @return int The current bottom lid state.
 */
int StateManager::getBottomLidState() const {
    return bottomLidState;
}

#ifdef SERIAL_DEBUG
/**
 * @brief Prints the current input values for debugging purposes.
 */
void StateManager::printDebugValues() {
    if (powerState && !sleeping) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer),
                "STATE: [MAN: %d | PAN: %4d | PTO: %3d | TILT: %4d | TTO: %3d | AL: %3d | TL: %3d | BL: %3d | AB: %d] ",
                inputHandler.isManualControlEnabled(), panState, panTwitchOffset, tiltState, tiltTwitchOffset, autoEyelidsState, topLidState, bottomLidState, autoBlinkState);
        Serial.print(buffer);
    } else {
        Serial.println("STATE: [POWER: OFF]");
    }
}
#endif