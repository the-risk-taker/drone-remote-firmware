/*
* Config.h
*
* Created: 17.07.2018 15:15:08
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/


#ifndef CONFIG_H_
#define CONFIG_H_

// SIMPLE TIMER
#define SIMPLE_TIMER_TIMEOUT_INTERVAL						10			// [ms]

// LONG RANGE RADIO
#define NRF24L01_DRONE_REMOTE_RADIO_IDENTIFIER				0
#define NRF24L01_DRONE_BASE_RADIO_IDENTIFIER				1

// JOYSTICK
#define JOYSTICK_PITCH_AXIS_MIN_VALUE						0
#define JOYSTICK_PITCH_AXIS_MAX_VALUE						1023

#define JOYSTICK_ROLL_AXIS_MIN_VALUE						0
#define JOYSTICK_ROLL_AXIS_MAX_VALUE						1023

#define JOYSTICK_YAW_AXIS_MIN_VALUE							63
#define JOYSTICK_YAW_AXIS_MAX_VALUE							947

#define JOYSTICK_THROTTLE_AXIS_MIN_VALUE					90
#define JOYSTICK_THROTTLE_AXIS_MAX_VALUE					950

#define PITCH_LOW_LIMIT										-45.0F		// degree
#define PITCH_HIGH_LIMIT									45.0F

#define ROLL_LOW_LIMIT										-45.0F		// degree
#define ROLL_HIGH_LIMIT										45.0F

#define YAW_LOW_LIMIT										-180.0F		// degree
#define YAW_HIGH_LIMIT										180.0F

#define THROTTLE_LOW_LIMIT									0			// microseconds
#define THROTTLE_HIGH_LIMIT									1000

// PINS
#define	PITCH_ANALOG_PIN									A0			// Y axis
#define	ROLL_ANALOG_PIN										A1			// X axis
#define	YAW_ANALOG_PIN										A2 			// X axis
#define	THROTTLE_ANALOG_PIN									A3 			// Y axis

#define NRF24L01_CE_PIN										9
#define NRF24L01_CSN_PIN									10

#endif /* CONFIG_H_ */