# Blinkenstein

Yet another arduino control codebase for the [Simplified 3D Printed Animatronic Dual Eye Mechanism](https://www.instructables.com/Simplified-3D-Printed-Animatronic-Dual-Eye-Mechani)

## Development notes
Much of this code was generated with the help of Github Co-Pilot in an attempt to better understand
how LLMs integrate into IDEs and where the line between help and hinderance is.

## Hardware
- M5Stamp C3
- PCA9685 16 Channel Servo Control Board
- Adafruit (clone) 2-axis Thumb Joystick with Select Button + Breakout Board
- Ouwei Type-C USB 5v 2A Step-Up Boost Converter Power Module LIPO Charging board
- 2400maH LIPO 1S battery
- Various resistors, capacitors, buttons, potentiometers etc... all outlined in the circuit diagram

## Debugging
To enable or disable debugging, comment or un-comment the `#define SERIAL_DEBUG` line in [config.h](src/config.h#L10)

## TODO
- Add the circuit diagram to the codebase
- Add photos to the `README.md`