#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

class MotorController
{
public:
	MotorController();
	void begin();
	void drive(int motor, int direction, int speed);
};

#endif