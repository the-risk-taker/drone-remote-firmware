/*
* Joystick.cpp
*
* Created: 17.07.2018 15:15:29
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "../Headers/Joystick.h"
#include "../Configs/Config.h"

Joystick::Joystick(String name, uint8_t xAxisPin, uint8_t yAxisPin) :
_name(name),
_xAxisPin(xAxisPin),
_yAxisPin(yAxisPin)
{
	
}

void Joystick::begin()
{
	this->init();
}

String Joystick::getName()
{
	return this->_name;
}

uint8_t Joystick::getXAxisPin()
{
	return this->_xAxisPin;
}

uint8_t Joystick::getYAxisPin()
{
	return this->_yAxisPin;
}

void Joystick::setXAxisMaxima(uint16_t min, uint16_t max)
{
	this->xAxisMaxima.min = min;
	this->xAxisMaxima.max = max;
}

void Joystick::setYAxisMaxima(uint16_t min, uint16_t max)
{
	this->yAxisMaxima.min = min;
	this->yAxisMaxima.max = max;
}

void Joystick::read(uint8_t numberOfSamples /*= 20*/)
{
	this->rawPosition.xAxis = this->movingAverage(this->xAxisBuffer, analogRead(this->_xAxisPin));
	this->rawPosition.yAxis = this->movingAverage(this->yAxisBuffer, analogRead(this->_yAxisPin));
}

sJoystickAxesInt Joystick::getRawPosition()
{
	return this->rawPosition;
}

float Joystick::getScaled(eAxisRange range)
{
	switch(range)
	{
		case PITCH:
		{
			return this->mapfloat(	(float)this->rawPosition.yAxis,
			(float)this->yAxisMaxima.min, (float)this->yAxisMaxima.max,
			JOYSTICK_PITCH_AXIS_MIN_VALUE, JOYSTICK_PITCH_AXIS_MAX_VALUE);
		}
		break;
		case ROLL:
		{
			return this->mapfloat(	(float)this->rawPosition.xAxis,
			(float)this->xAxisMaxima.min, (float)this->xAxisMaxima.max,
			JOYSTICK_ROLL_AXIS_MIN_VALUE, JOYSTICK_ROLL_AXIS_MAX_VALUE);
		}
		break;
		case YAW:
		{
			return this->mapfloat(	(float)this->rawPosition.xAxis,
			(float)this->xAxisMaxima.min, (float)this->xAxisMaxima.max,
			YAW_LOW_LIMIT, YAW_HIGH_LIMIT);
		}
		break;
		case THROTTLE:
		{
			return this->mapfloat(	(float)this->rawPosition.yAxis,
			(float)this->yAxisMaxima.min, (float)this->yAxisMaxima.max,
			THROTTLE_LOW_LIMIT, THROTTLE_HIGH_LIMIT);
		}
		break;
		default:
		{
			// TODO;
			return -1;
		}
		break;
	}
}

void Joystick::init()
{
	pinMode(this->_xAxisPin, INPUT);
	pinMode(this->_yAxisPin, INPUT);
}

float Joystick::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
	float returnValue = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	
	if(returnValue > out_max)
	{
		returnValue = out_max;
	}
	else if(returnValue < out_min)
	{
		returnValue = out_min;
	}
	
	return returnValue;
}

uint16_t Joystick::movingAverage(uint16_t *buffer, uint16_t newValue)
{
	uint32_t sum = 0;
	
	buffer[0] = newValue;
	
	for(uint8_t i = 0; i < MOVING_AVERAGE_RANGE; i++)
	{
		sum += buffer[i];
	}
	
	this->moveElements(buffer, MOVING_AVERAGE_RANGE - 1);

	return (uint16_t)(sum/MOVING_AVERAGE_RANGE);
}

void Joystick::moveElements(uint16_t table[], uint8_t lastElementIndex)
{
	for(uint8_t i = lastElementIndex; i > 0; i--)
	{
		table[i] = table[i - 1];
	}
}

Joystick PitchRoll("Pitch and Roll", PITCH_ANALOG_PIN, ROLL_ANALOG_PIN);
Joystick ThrottleYaw("Throttle and Yaw", THROTTLE_ANALOG_PIN, YAW_ANALOG_PIN);