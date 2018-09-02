/* 
* ProtocolAPI.cpp
*
* Created: 09.07.2018 15:41:10
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/

#include "../Headers/ProtocolAPI.h"

 ProtocolAPI::ProtocolAPI(const String protocolName, HardwareSerial& UARTObject, uint32_t baudrate, eCommInterfaceType interfaceType /*= SERIAL_INTERFACE*/)
{
	this->protocolPackets = new ProtocolPacketsProcessing(protocolName,UARTObject, baudrate, interfaceType);
}

 ProtocolAPI::ProtocolAPI(const String protocolName, eCommInterfaceType interfaceType /*= NATIVE_INTERFACE*/)
{
	this->protocolPackets = new ProtocolPacketsProcessing(protocolName, interfaceType);
}

uint8_t ProtocolAPI::begin()
{
	return this->protocolPackets->protocolBytes->begin();
}

void ProtocolAPI::processPacket()
{
	if(this->protocolPackets->getNewPacketsNumber())
	{
		for(uint8_t i = 0; i < this->protocolPackets->getNewPacketsNumber(); i++)
		{
			sProtocolPacket incomingPacket = this->protocolPackets->getNewPacketsTable()[i];

			this->encodePacket(incomingPacket);
		}
		
		this->protocolPackets->resetNewPacketsNumber();
	}
}

void ProtocolAPI::sendPacket(eCommandCodes command)
{
	sProtocolPacket outcomingPacket;
	
	outcomingPacket.commandCode = (uint8_t)command;
	outcomingPacket.length = 0;
	
	this->processOutcommingPacketToBytesAndSend(outcomingPacket);
}

eConnectionStatus ProtocolAPI::getConnectionStatus()
{
	return this->protocolPackets->protocolBytes->getConnectionFlag();
}

void ProtocolAPI::processOutcommingPacketToBytesAndSend(sProtocolPacket packet)
{
	if(this->getConnectionStatus() == CONNECTED)
	{
		this->protocolPackets->rewritePacketToBytes(packet);
		this->protocolPackets->protocolBytes->writeBytes();
	}
	else if((eCommandCodes)packet.commandCode == INIT_COMM)
	{
		this->protocolPackets->rewritePacketToBytes(packet);
		this->protocolPackets->protocolBytes->writeBytes();
	}
	else
	{
		Serial.println(String(millis()) + "\tThere is no connection!");
	}
}

void ProtocolAPI::encodePacket(sProtocolPacket packet)
{
	// TODO uncomment to print packet
	//this->protocolPackets->printPacket(packet);
	
	eCommandCodes code = (eCommandCodes)packet.commandCode;
	
	Serial.println(packet.commandCode);
	
	switch(code)
	{
		case NO_CODE:
		{
			//Serial.println("NO_CODE");
		}
		break;
		case ERROR:
		{
			//Serial.println("ERROR");
		}
		break;
		case WARNING:
		{
			//Serial.println("WARNING");
		}
		break;
		case INIT_COMM:
		{
			this->sendPacket(COMM_STARTED);
			
			//Serial.println("INIT_COMM");
		}
		break;
		case STOP_COMM:
		{
			//Serial.println("STOP_COMM");
		}
		break;
		case COMM_STARTED:
		{
			this->protocolPackets->protocolBytes->setConnectionStatus(CONNECTED);
		}
		break;
		case COMM_ENDED:
		{
			//Serial.println("COMM_ENDED");
		}
		break;
		case ACKNOWLEDGEMENT:
		{
			//Serial.println("ACKNOWLEDGEMENT");
		}
		break;
		default:
		{
			//Serial.println("\tERROR\tUnknown command code!");
		}
		break;
	}
}

ProtocolAPI LongRangeComm("Long Range Communication", NATIVE_INTERFACE);