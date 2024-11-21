#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <SPI.h>

// ESP Pin definitions
#define PIN_JOYSTICK_X 1       // GPIO1 for X-axis (ADC1_CH1)
#define PIN_JOYSTICK_Y 0       // GPIO0 for Y-axis (ADC1_CH0)
#define PIN_BLINK_BUTTON 10    // Digital pin for Blink Button
#define PIN_EYELIDS_POT 4      // Potentiometer for Eyelids Closed (ADC1_CH4)
#define PIN_SDA 8              // GPIO8 for I2C SDA
#define PIN_SCL 9              // GPIO9 for I2C SCL

// Servo PWM settings
#define SERVO_PWM_FREQ 60  // Analog servos run at ~60 Hz

// Minimum and maximum pulse width for servos
#define SERVO_PAN_MIN 270
#define SERVO_PAN_MAX 520
#define SERVO_TILT_MIN 350                // Lower = more down
#define SERVO_TILT_MAX 580                // Higher = more up
#define SERVO_LEFT_LID_TOP_OPEN 180       // Lower = more open
#define SERVO_LEFT_LID_TOP_CLOSED 340     // Higher = more closed
#define SERVO_LEFT_LID_BOTTOM_OPEN 550    // Higher = more open
#define SERVO_LEFT_LID_BOTTOM_CLOSED 300  // Lower = more closed
#define SERVO_RIGHT_LID_TOP_OPEN 600      // Higher = more open
#define SERVO_RIGHT_LID_TOP_CLOSED 400    // Lower = more closed
#define SERVO_RIGHT_LID_BOTTOM_OPEN 250   // Lower = more open
#define SERVO_RIGHT_LID_BOTTOM_CLOSED 500 // Higher = more closed  

// Servo PWM channels
#define SERVO_CHANNEL_PAN 5
#define SERVO_CHANNEL_TILT 4
#define SERVO_CHANNEL_LEFT_LID_TOP 8
#define SERVO_CHANNEL_LEFT_LID_BOTTOM 9
#define SERVO_CHANNEL_RIGHT_LID_TOP 6
#define SERVO_CHANNEL_RIGHT_LID_BOTTOM 7

// Smoothing factor for exponential moving average (0 < alpha <= 1)
#define SMOOTHING_FACTOR 0.1

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Variables to store logical values
int joystickPan = 0;
int joystickTilt = 0;
int eyelidsClosedAmount = 0;      // 0 = open, 100 = closed
int blinkButtonPressed = 0; // 0 = not pressed, 1 = pressed

// Variables to store servo PWM values
int servoPanPulse = 0;
int servoTiltPulse = 0;
int servoLeftLidTopPulse = 0;
int servoLeftLidBottomPulse = 0;
int servoRightLidTopPulse = 0;
int servoRightLidBottomPulse = 0;

// Function prototypes
void readInputValues();
void calculatePWMValues();
void updateServos();
void printValues();

// Setup function
void setup() {
  Serial.begin(115200);
  pinMode(PIN_BLINK_BUTTON, INPUT_PULLUP);
  pinMode(PIN_JOYSTICK_X, INPUT);
  pinMode(PIN_JOYSTICK_Y, INPUT);
  pinMode(PIN_EYELIDS_POT, INPUT);

  // Initialize I2C with specific SDA and SCL pins
  Wire.begin(PIN_SDA, PIN_SCL);

  // Initialize the PCA9685 board
  pwm.begin();
  pwm.setPWMFreq(SERVO_PWM_FREQ);

  // Debug information
  Serial.println("Setup complete. Starting loop...");
}

// Main loop
void loop() {
  // Read the input values
  readInputValues();

  // Calculate the new PWM values
  calculatePWMValues();

  // Process the new inputs and update the servos
  updateServos();

  // Output debug information
  printValues();

  delay(1);
}

void readInputValues()
{
  // Read the pin values
  int panValue = constrain(analogRead(PIN_JOYSTICK_X), 0, 4095);
  int tiltValue = constrain(analogRead(PIN_JOYSTICK_Y), 0, 4095);
  int potValue = constrain(analogRead(PIN_EYELIDS_POT), 0, 4095);
  int blinkButtonValue = digitalRead(PIN_BLINK_BUTTON);

  // Map the joystick values to a range of -100 to 100
  float newJoystickPan = map(panValue, 0, 4095, -100, 100);
  float newJoystickTilt = map(tiltValue, 0, 4095, -100, 100);

  // Map the pot value to a range of 0 to 100
  float newPotValue = map(potValue, 0, 4095, 0, 100);

  // Apply exponential moving average to smooth the joystick values
  if (newJoystickPan > (SMOOTHING_FACTOR * 100) || newJoystickPan < -(SMOOTHING_FACTOR * 100)) {
    joystickPan = newJoystickPan;
  } else {
    joystickPan = (SMOOTHING_FACTOR * newJoystickPan) + ((1 - SMOOTHING_FACTOR) * joystickPan);
  }

  if (newJoystickTilt > (SMOOTHING_FACTOR * 100) || newJoystickTilt < -(SMOOTHING_FACTOR * 100)) {
    joystickTilt = newJoystickTilt;
  } else {
    joystickTilt = (SMOOTHING_FACTOR * newJoystickTilt) + ((1 - SMOOTHING_FACTOR) * joystickTilt);
  }

  if (newPotValue > (SMOOTHING_FACTOR * 100)) {
    eyelidsClosedAmount = newPotValue;
  } else {
    eyelidsClosedAmount = (SMOOTHING_FACTOR * newPotValue) + ((1 - SMOOTHING_FACTOR) * eyelidsClosedAmount);
  }

  blinkButtonPressed = !blinkButtonValue;
}

void calculatePWMValues() {
  // Map the inverted PAN joystick value to the servo pan pulse range
  servoPanPulse = map(joystickPan * -1, -100, 100, SERVO_PAN_MIN, SERVO_PAN_MAX);
  // Map the TILT joystick value to the servo tilt pulse range
  servoTiltPulse = map(joystickTilt * -1, -100, 100, SERVO_TILT_MIN, SERVO_TILT_MAX);

  int newEyeLidsClosedAmount = blinkButtonPressed ? 100 : eyelidsClosedAmount;

  servoLeftLidTopPulse = map(newEyeLidsClosedAmount, 0, 100, SERVO_LEFT_LID_TOP_OPEN, SERVO_LEFT_LID_TOP_CLOSED);
  servoLeftLidBottomPulse = map(newEyeLidsClosedAmount, 0, 100, SERVO_LEFT_LID_BOTTOM_OPEN, SERVO_LEFT_LID_BOTTOM_CLOSED);
  servoRightLidTopPulse = map(newEyeLidsClosedAmount, 0, 100, SERVO_RIGHT_LID_TOP_OPEN, SERVO_RIGHT_LID_TOP_CLOSED);
  servoRightLidBottomPulse = map(newEyeLidsClosedAmount, 0, 100, SERVO_RIGHT_LID_BOTTOM_OPEN, SERVO_RIGHT_LID_BOTTOM_CLOSED);
}

void updateServos() {
  // Set the servo pulse width on the specified channels
  pwm.setPWM(SERVO_CHANNEL_PAN, 0, servoPanPulse);
  pwm.setPWM(SERVO_CHANNEL_TILT, 0, servoTiltPulse);

  pwm.setPWM(SERVO_CHANNEL_LEFT_LID_TOP, 0, servoLeftLidTopPulse);
  pwm.setPWM(SERVO_CHANNEL_LEFT_LID_BOTTOM, 0, servoLeftLidBottomPulse);
  pwm.setPWM(SERVO_CHANNEL_RIGHT_LID_TOP, 0, servoRightLidTopPulse);
  pwm.setPWM(SERVO_CHANNEL_RIGHT_LID_BOTTOM, 0, servoRightLidBottomPulse);
}

void printValues() {
  // Buffer to hold the formatted string
  char buffer[256];

  // Format the values and concatenate them into the buffer
  snprintf(buffer, sizeof(buffer),
           "INPUT: [PAN: %4d | TILT: %4d | Eyelids: %3d | Blink: %1d] SERVOS: [PAN: %3d | TILT: %3d | LLT: %3d | LLB: %3d | RLT: %3d | RLB: %3d]",
           (int)joystickPan, (int)joystickTilt, (int)eyelidsClosedAmount, (int)blinkButtonPressed,
           servoPanPulse, servoTiltPulse, servoLeftLidTopPulse, servoLeftLidBottomPulse, servoRightLidTopPulse, servoRightLidBottomPulse);

  // Print the formatted string
  Serial.println(buffer);
}