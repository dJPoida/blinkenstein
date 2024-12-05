#ifndef BRAIN_H
#define BRAIN_H

#include <Arduino.h>
#include "inputHandler.h"

class Brain {
public:
    Brain(InputHandler& inputHandler);

    void begin();
    void update();

    int getPanState() const;
    int getTiltState() const;
    int getEyelidsState() const;
    bool getBlinkState() const;

    bool isManualControlEnabled() const;

    void printDebugValues();
private:
    InputHandler& inputHandler;

    int panState;
    int tiltState;
    int eyelidsState;
    bool blinkState;

    unsigned long previousUpdateMillis;
    unsigned long perviousBlinkMillis;
    unsigned long updateInterval;

    bool manualControlEnabled;

    void randomizeStates();
};

extern Brain brain;

#endif // BRAIN_H