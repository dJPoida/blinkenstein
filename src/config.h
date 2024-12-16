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
#define DEBUG_BRAIN     0       // Output the brain values to the serial monitor
#define DEBUG_STATE     1       // Output the state values to the serial monitor
#define DEBUG_SERVOS    0       // Output the servo values to the serial monitor

// ESP Pin definitions
#define PIN_POWER_BUTTON 6      // Digital pin for Software Power (when charging, the ESP will be powered on, sotware power state prevents autonomous control)
#define PIN_JOYSTICK_X 0        // Joystic X-axis
#define PIN_JOYSTICK_Y 1        // Joystick Y-axis
#define PIN_BLINK_BUTTON 10     // Digital pin for Primary Blink Button
#define PIN_BLINK_BUTTON_2 5    // Digital pin for Secondary Blink Button
#define PIN_EYELIDS_POT 4       // Potentiometer for Eyelids Closed
#define PIN_SDA 9               // I2C SDA
#define PIN_SCL 8               // I2C SCL

// Servo PWM settings
#define SERVO_I2C_ADDRESS 0x40  // Default PCA9685 I2C address
#define SERVO_PWM_FREQ 60       // Analog servos run at ~60 Hz

// Minimum and maximum pulse width for servos
#define SERVO_PAN_MIN 270                   // Lower = more left, Higher = more right
#define SERVO_PAN_MAX 520                   // Lower = more left, Higher = more right
#define SERVO_TILT_MIN 200                  // Lower = more down, Higher = more up
#define SERVO_TILT_MAX 360                  // Lower = more down, Higher = more up
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

// When should manual control be enabled
#define MANUAL_CONTROL_ENABLED_DEFAULT 0         // Enable manual control by default
#define MANUAL_CONTROL_INTERRUPT_THRESHOLD 1000  // What change in value on the analog inputs will trigger manual control
#define MANUAL_CONTROL_TIMEOUT 30000             // How long to wait before reverting to autonomous control (ms)

#define BRAIN_UPDATE_INTERVAL 100               // How often the brain should consider changing the states (ms)
#define BRAIN_MAX_CHANCE 100                    // The maximum chance value for random events
#define BRAIN_CHANCE_OF_BLINK 2                 // The chance of performing a blink (0 -> BRAIN_MAX_CHANCE)
#define BRAIN_CHANCE_OF_EYELID_CHANGE 1         // The chance of the eyelids changing (0 -> BRAIN_MAX_CHANCE)
#define BRAIN_CHANCE_OF_MAJOR_LOOK_CHANGE 2    // The chance of the eyeballs changing direction slightly (0 -> BRAIN_MAX_CHANCE)
#define BRAIN_CHANCE_OF_MINOR_LOOK_CHANGE 10     // The chance of the eyeballs changing direction in a large way (0 -> BRAIN_MAX_CHANCE)
#define BRAIN_CHANCE_OF_LOOK_RETURN_CENTRE 25   // The chance of the eyeballs returning to the centre (0 -> BRAIN_MAX_CHANCE)
#define BRAIN_CHANCE_OF_BLINK_WHILE_LOOK 25     // The chance of blinking while looking around (0 -> BRAIN_MAX_CHANCE)

// Pupeteering Settings
// --------------------

// Prevent the pupil from be obscured by the eyelids
#define PUPIL_REVEAL_LID_MIN_AMOUNT 20
#define PUPIL_REVEAL_LID_MAX_AMOUNT 58

#define BLINK_DURATION 150                   // How long the blink should last (when the brain is in control) (ms)
