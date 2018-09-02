/* 
* Joystick.h
*
* Created: 17.07.2018 15:15:29
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#define MOVING_AVERAGE_RANGE					5 // samples

#ifndef JOYSTICK_STRUCTURES_ENUMS
#define JOYSTICK_STRUCTURES_ENUMS

typedef struct sJoystickAxesInt
{
	uint16_t xAxis;
	uint16_t yAxis;
} sJoystickAxesInt;

typedef struct sJoystickAxesFloat
{
	float xAxis;
	float yAxis;
} sJoystickAxesFloat;

typedef struct sJoystickMaxima
{
	uint16_t min;
	uint16_t max;
} sJoystickMaxima;

typedef enum eAxisType
{
	X,
	Y
} eAxisType;

typedef enum eAxisRange
{
	PITCH,
	ROLL,
	YAW,
	THROTTLE	
} eAxisRange;

#endif

class Joystick
{
public:
	Joystick(String name, uint8_t xAxisPin, uint8_t yAxisPin);
	
	void begin();
	
	String getName();
	uint8_t getXAxisPin();
	uint8_t getYAxisPin();
	
	void setXAxisMaxima(uint16_t min, uint16_t max);
	void setYAxisMaxima(uint16_t min, uint16_t max);
	
	void read(uint8_t numberOfSamples = MOVING_AVERAGE_RANGE);
	
	sJoystickAxesInt getRawPosition();
	float getScaled(eAxisRange range);
	
private:
	String _name;
	
	const uint8_t	_xAxisPin,   
					_yAxisPin;
					
	sJoystickAxesInt	rawPosition;
	
	sJoystickMaxima xAxisMaxima,
					yAxisMaxima;
					
	uint16_t xAxisBuffer[MOVING_AVERAGE_RANGE];
	uint16_t yAxisBuffer[MOVING_AVERAGE_RANGE];
					
	void init();
	float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
	
	uint16_t movingAverage(uint16_t *buffer, uint16_t newValue);
	void moveElements(uint16_t table[], uint8_t lastElementIndex);
};

extern Joystick PitchRoll;
extern Joystick ThrottleYaw;

#endif //__JOYSTICK_H__
