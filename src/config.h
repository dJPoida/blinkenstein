/**
 * @file config.h
 * @brief Configuration settings for the Blinkenstein project.
 *
 * This file contains various configuration settings, including debug options,
 * pin definitions, servo settings, and other constants used throughout the project.
 */

// Uncomment the following line to enable debug output
#define SERIAL_DEBUG

// Debug Config
#define DEBUG_INTERVAL 50       // How often the debug information should be printed to the serial monitor (ms)
#define DEBUG_INPUT     1       // Output the input values to the serial monitor
#define DEBUG_STATE     1       // Output the state values to the serial monitor
#define DEBUG_SERVOS    0       // Output the servo values to the serial monitor

// ESP Pin definitions
#define PIN_JOYSTICK_X 0        // Joystic X-axis
#define PIN_JOYSTICK_Y 1        // Joystick Y-axis
#define PIN_BLINK_BUTTON 10     // Digital pin for Primary Blink Button
#define PIN_BLINK_BUTTON_2 5    // Digital pin for Secondary Blink Button
#define PIN_EYELIDS_POT 4       // Potentiometer for Eyelids Closed
#define PIN_SDA 9               // I2C SDA
#define PIN_SCL 8               // I2C SCL

// Servo PWM settings
#define SERVO_PWM_FREQ 60       // Analog servos run at ~60 Hz

// Minimum and maximum pulse width for servos
#define SERVO_PAN_MIN 270                   // Lower = more left, Higher = more right
#define SERVO_PAN_MAX 520                   // Lower = more left, Higher = more right
#define SERVO_TILT_MIN 350                  // Lower = more down, Higher = more up
#define SERVO_TILT_MAX 580                  // Lower = more down, Higher = more up
#define SERVO_LEFT_LID_TOP_OPEN 180         // Lower = more open, Higher = more closed
#define SERVO_LEFT_LID_TOP_CLOSED 340       // Lower = more open, Higher = more closed
#define SERVO_LEFT_LID_BOTTOM_OPEN 550      // Lower = more closed, Higher = more open
#define SERVO_LEFT_LID_BOTTOM_CLOSED 290    // Lower = more closed, Higher = more open
#define SERVO_RIGHT_LID_TOP_OPEN 600        // Lower = more closed, Higher = more open
#define SERVO_RIGHT_LID_TOP_CLOSED 400      // Lower = more closed, Higher = more open
#define SERVO_RIGHT_LID_BOTTOM_OPEN 250     // Lower = more open, Higher = more closed
#define SERVO_RIGHT_LID_BOTTOM_CLOSED 500   // Lower = more open, Higher = more closed

// Servo PWM channels
#define SERVO_CHANNEL_PAN 4
#define SERVO_CHANNEL_TILT 5
#define SERVO_CHANNEL_LEFT_LID_TOP 8
#define SERVO_CHANNEL_LEFT_LID_BOTTOM 9
#define SERVO_CHANNEL_RIGHT_LID_TOP 10
#define SERVO_CHANNEL_RIGHT_LID_BOTTOM 11

// Smoothing factor for exponential moving average (0 < alpha <= 1)
#define SMOOTHING_FACTOR 0.05

// Compensate for joystick drift (the joystick may not center at 0)
#define JOYSTICK_DRIFT_ADUSTMENT_X 100  // -2048 -> 2048
#define JOYSTICK_DRIFT_ADUSTMENT_Y 125  // -2048 -> 2048
#define JOYSTICK_DEADZONE 100           // (0 -> 2048) Any value below this threshold will be ignored

// Pupeteering Settings
// --------------------

// Prevent the pupil from be obscured by the eyelids
#define PUPIL_REVEAL_LID_MIN_AMOUNT 20
#define PUPIL_REVEAL_LID_MAX_AMOUNT 58
