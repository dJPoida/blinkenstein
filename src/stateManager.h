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
#include "brain.h"

class StateManager {
public:
    StateManager(InputHandler& inputHandler, Brain& brain);

    void update();

    int getPanState() const;
    int getTiltState() const;
    int getTopLidState() const;
    int getBottomLidState() const;

    void printDebugValues();
private:
    InputHandler& inputHandler;
    Brain& brain;

    int panState;
    int tiltState;
    int topLidState;
    int bottomLidState;

    bool manualControl;
};

extern StateManager stateManager;

#endif // STATE_MANAGER_H