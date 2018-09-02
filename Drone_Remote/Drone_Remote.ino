/*
* Drone_Remote.ino
*
* Created: 7/17/2018 3:08:01 PM
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Release to public domain under MIT License, unless it states otherwise. Check files notes.
*/

#include <avr/wdt.h>
#include "Configs/Config.h"
#include "Headers/Joystick.h"
#include "Headers/NRFLite.h"
#include "Headers/ProtocolAPI.h"

typedef enum
{
	INIT_HARDWARE_STATE,
	INIT_SENSORS_STATE,
	INIT_COMM_STATE,
	IDLE_STATE,
	SEND_CONTROL_VALUES_STATE
} eRemoteStatesMachine;

eRemoteStatesMachine RemoteStatesMachine = INIT_HARDWARE_STATE;

void setup()
{
	Serial.begin(115200);
	Serial.println(String(millis()) + "\tSerial init");
	
	while(!LongRangeComm.begin())
	{
		Serial.println(String(millis()) + "\tRadio is not working!");
		delay(250);
	}
	
	Serial.println(String(millis()) + "\tRadio is working!");
	
	ThrottleYaw.begin();
	
	wdt_enable(WDTO_120MS);
}

void loop()
{
	LongRangeComm.protocolPackets->protocolBytes->readBytes();
	processBytes();
	
	if(timerTimeout(SIMPLE_TIMER_TIMEOUT_INTERVAL))
	{
		switch(RemoteStatesMachine)
		{
			case INIT_HARDWARE_STATE:
			{
				RemoteStatesMachine = INIT_SENSORS_STATE;
			}
			break;
			case INIT_SENSORS_STATE:
			{
				ThrottleYaw.setYAxisMaxima(JOYSTICK_THROTTLE_AXIS_MIN_VALUE, JOYSTICK_THROTTLE_AXIS_MAX_VALUE);
				
				RemoteStatesMachine = INIT_COMM_STATE;
			}
			break;
			case INIT_COMM_STATE:
			{
				LongRangeComm.sendPacket(INIT_COMM);
				
				RemoteStatesMachine = IDLE_STATE;
			}
			break;
			case IDLE_STATE:
			{
				readSensors();
				
				static uint32_t prev = millis(); 
				if(LongRangeComm.getConnectionStatus() == CONNECTED)
				{
					RemoteStatesMachine = SEND_CONTROL_VALUES_STATE;
				}
				else if((millis() - prev) >= 50)
				{
					LongRangeComm.sendPacket(INIT_COMM);
					prev = millis();
					
					Serial.println(String(millis()) +	"\tTrying to establish connection...\tT = " + (uint16_t)ThrottleYaw.getScaled(THROTTLE) + 
														"\tY = " + ThrottleYaw.getScaled(YAW) + 
														"\tP = " + PitchRoll.getScaled(PITCH) +
														"\tR = " + PitchRoll.getScaled(ROLL));
				}
			}
			break;
			case SEND_CONTROL_VALUES_STATE:
			{
				readSensors();
				
				// TODO send all control values
				uint16_t data = (uint16_t)ThrottleYaw.getScaled(THROTTLE);
				
				Serial.println(String(millis()) +	"\tSending...\tT = " + (uint16_t)ThrottleYaw.getScaled(THROTTLE) +
													"\tY = " + ThrottleYaw.getScaled(YAW) +
													"\tP = " + PitchRoll.getScaled(PITCH) +
													"\tR = " + PitchRoll.getScaled(ROLL));
													
				LongRangeComm.sendPacket(JOYSTICK_DATA, &data, sizeof(data));
			}
			break;
			default:
			{
				// TODO
			}
			break;
		}
	}

	wdt_reset();
}

void readSensors()
{
	readJoysticks();
}

void readJoysticks()
{
	ThrottleYaw.read();
	PitchRoll.read();
}

bool timerTimeout(uint32_t interval)
{
	static uint32_t previousTimerTime = millis();
	uint32_t actualTimerTime = millis();
	
	bool returnedStatus = false;
	
	if((actualTimerTime - previousTimerTime) >= interval)
	{
		returnedStatus = true;
		previousTimerTime = actualTimerTime;
	}
	
	return returnedStatus;
}

void processBytes()
{
	LongRangeComm.protocolPackets->processInputBytes();
	LongRangeComm.processPacket();
}