/**
 * @file debug.h
 * @brief Provides debugging utilities for the application.
 *
 * This file contains declarations for debugging functions and variables
 * that are used to print debug information when SERIAL_DEBUG is enabled.
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef SERIAL_DEBUG
#include <Arduino.h>

extern unsigned long previousDebugMillis;
void printDebugValues();

#endif // SERIAL_DEBUG

#endif // DEBUG_H