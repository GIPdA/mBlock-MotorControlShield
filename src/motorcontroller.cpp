#include "motorcontroller.h"

enum ArduinoPin {
  // Arduino pins for the shift register
  SR_Latch = 12,
  SR_Clock = 4,
  SR_Enable = 7,
  SR_Data = 8,

  SR_Motor1Pwm = 11,
  SR_Motor2Pwm = 3,
  SR_Motor3Pwm = 6,
  SR_Motor4Pwm = 5
};


// 8-bit bus after the 74HC595 shift register 
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
enum MotorBitOutput {
  Motor1_A = 2,
  Motor1_B = 3,

  Motor2_A = 1,
  Motor2_B = 4,

  Motor3_A = 5,
  Motor3_B = 7,

  Motor4_A = 0,
  Motor4_B = 6,
};


static int latch_value = 0;

static int const motorMap[] = {
  Motor1_A,Motor1_B,
  Motor2_A,Motor2_B, 
  Motor3_A,Motor3_B, 
  Motor4_A,Motor4_B
};

static int const motorPwmMap[] = {
  SR_Motor1Pwm,
  SR_Motor2Pwm, 
  SR_Motor3Pwm, 
  SR_Motor4Pwm
};


static void writeShiftValue(int output, int high_low)
{
  // The defines HIGH and LOW are 1 and 0.
  // So this is valid.
  bitWrite(latch_value, output, high_low);
}

static void shiftWrite()
{
  // Use the default Arduino 'shiftOut()' function to
  // shift the bits with the MOTORCLK as clock pulse.
  // The 74HC595 shiftregister wants the MSB first.
  // After that, generate a latch pulse with MOTORLATCH.
  shiftOut(SR_Data, SR_Clock, MSBFIRST, latch_value);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(SR_Latch, HIGH);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(SR_Latch, LOW);
}


MotorController::MotorController()
{
}

void MotorController::begin()
{
  // Set pins for shift register to output
  pinMode(SR_Latch, OUTPUT);
  pinMode(SR_Clock, OUTPUT);
  pinMode(SR_Enable, OUTPUT);
  pinMode(SR_Data, OUTPUT);

  // Set pins for shift register to default value (low);
  digitalWrite(SR_Data, LOW);
  digitalWrite(SR_Latch, LOW);
  digitalWrite(SR_Clock, LOW);

  // Enable the shift register, set Enable pin Low.
  digitalWrite(SR_Enable, LOW);
}

void MotorController::drive(int motor, int direction, int speed)
{
  if (motor <= 0 || motor > 4)
    return;

  direction = (direction == 0 ? 0 : 1);

  int const motorMapIndex = (motor-1)*2;
  int const pwmOutput = motorPwmMap[motor-1];


  writeShiftValue(motorMap[ motorMapIndex + 0 ], direction);
  writeShiftValue(motorMap[ motorMapIndex + 1 ], !direction);

  shiftWrite();

  //if (speed >= 0 && speed <= 255)
    analogWrite(pwmOutput, speed);
}
