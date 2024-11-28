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

class StateManager {
public:
    StateManager();

    void update(int joystickXPercent, int joystickYPercent, int potPercent, bool buttonPressed);

    int getPanState() const;
    int getTiltState() const;
    int getLeftLidTopState() const;
    int getLeftLidBottomState() const;
    int getRightLidTopState() const;
    int getRightLidBottomState() const;

private:
    int panState;
    int tiltState;
    int leftLidTopState;
    int leftLidBottomState;
    int rightLidTopState;
    int rightLidBottomState;
};

extern StateManager stateManager;

#endif