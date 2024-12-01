/**
 * @file stateManager.h
 * @brief Manages the state of the Blinkenstein project.
 *
 * This class is responsible for maintaining and updating the state of various components,
 * such as the positions of servos and the state of inputs. It provides methods to access
 * the current state and update it based on input values.
 */

#include <Arduino.h>
#include "config.h"
#include "StateManager.h"

/**
 * @brief Constructs a new StateManager object.
 *
 * @param inputHandler Reference to the InputHandler object.
 */
StateManager::StateManager(InputHandler& inputHandler)
    : inputHandler(inputHandler), panState(0), tiltState(0), topLidState(0), bottomLidState(0)
{}


/**
 * @brief Updates the state based on the current input values.
 *
 * @todo eventually this will be an update based on the user inputs OR the autonomous brain control
 */
void StateManager::update() {
    // Get the current input values
    int joystickXPercent = inputHandler.getJoystickXPercent();
    int joystickYPercent = inputHandler.getJoystickYPercent();
    int potPercent = inputHandler.getPotPercent();
    bool buttonPressed = inputHandler.getButtonPressed();

    int newPanState = joystickXPercent;
    int newTiltState = joystickYPercent;

    // Set the initial lid state based on the blink button or the potentiometer
    int newTopLidState = buttonPressed ? 0 : potPercent;
    int newBottomLidState = buttonPressed ? 0 : potPercent;

    if (newTiltState < 0 and newTopLidState > PUPIL_REVEAL_LID_MIN_AMOUNT and newTopLidState < PUPIL_REVEAL_LID_MAX_AMOUNT) {
        int offsetTopLidState = map(-newTiltState, 0, 100, PUPIL_REVEAL_LID_MIN_AMOUNT, PUPIL_REVEAL_LID_MAX_AMOUNT) - PUPIL_REVEAL_LID_MIN_AMOUNT;
        newTopLidState = constrain(newTopLidState + offsetTopLidState, 0, 100);
    }

    if (newTiltState > 0 and newBottomLidState > PUPIL_REVEAL_LID_MIN_AMOUNT and newBottomLidState < PUPIL_REVEAL_LID_MAX_AMOUNT) {
        int offsetBottomLidState = map(-newTiltState, 0, 100, PUPIL_REVEAL_LID_MIN_AMOUNT, PUPIL_REVEAL_LID_MAX_AMOUNT) - PUPIL_REVEAL_LID_MIN_AMOUNT;
        newBottomLidState = constrain(newBottomLidState - offsetBottomLidState, 0, 100);
    }

    panState = newPanState;
    tiltState = newTiltState;
    topLidState = newTopLidState;
    bottomLidState = newBottomLidState;
}

/**
 * @brief Gets the current pan state.
 *
 * @return int The current pan state.
 */
int StateManager::getPanState() const {
    return panState;
}

/**
 * @brief Gets the current tilt state.
 *
 * @return int The current tilt state.
 */
int StateManager::getTiltState() const {
    return tiltState;
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

/**
 * @brief Prints the current input values for debugging purposes.
 */
void StateManager::printDebugValues() {
    char buffer[256];
    snprintf(buffer, sizeof(buffer),
            "STATE: [PAN: %4d | TILT: %4d | TOP LID: %4d | BOT LID: %4d] ",
            panState, tiltState, topLidState, bottomLidState);
    Serial.print(buffer);
}
