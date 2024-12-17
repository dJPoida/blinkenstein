/**
 * @file stateManager.h
 * @brief Manages the state of the Blinkenstein project.
 *
 * This class is responsible for maintaining and updating the state of various components,
 * such as the positions of servos and the state of inputs. It provides methods to access
 * the current state and update it based on input values.
 */

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>
#include "inputHandler.h"

class StateManager {
public:
    StateManager(InputHandler& inputHandler);

    void begin();
    void update();

    bool getPowerState() const;
    void setPowerState(bool state);

    int getPanState() const;
    int getTiltState() const;
    int getTopLidState() const;
    int getBottomLidState() const;

    #ifdef SERIAL_DEBUG
    void printDebugValues();
    #endif

private:
    InputHandler& inputHandler;

    bool powerState;

    int panState;
    int tiltState;
    int panTwitchOffset;
    int tiltTwitchOffset;
    int topLidState;
    int bottomLidState;

    bool sleeping;

    int autoEyelidsState;
    bool autoBlinkState;

    unsigned long previousAutoUpdateMillis;
    unsigned long perviousAutoBlinkMillis;

    bool checkPowerState();
    void randomizeStates(int& newPanState, int& newTiltState, int& newTopLidState, int& newBottomLidState, int& newAutoEyelidsState, bool& newAutoBlinkState);
    void powerDown();
};;

extern StateManager stateManager;

#endif // STATE_MANAGER_H