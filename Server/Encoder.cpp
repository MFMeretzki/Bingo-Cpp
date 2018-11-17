#include "Encoder.h"


std::string Encoder::EncodeBase(unsigned short commandId)
{
	std::string command;
	command += commandId;
	
	return command;
}


std::string Encoder::EncodeUShort(unsigned short commandId, unsigned short value)
{
	std::string command;
	command += commandId;
	command += value;
	
	return command;
}


std::string Encoder::EncodeUShort3(unsigned short commandId, unsigned short value[3])
{
	std::string command;
	command += commandId;
	
	for (int i=0; i<3; ++i)
	{
		command += value[i];
	}
	
	return command;
}


std::string Encoder::EncodeCards(unsigned short commandId, unsigned short nCards, Card card[])
{
	std::string command;
	command += commandId;
	command += nCards;
	
	int i;
	for (i=0; i<nCards; ++i)
	{
		command += card[i].StringCode();
	}
	
	return command;
}


bool Encoder::DecodeUShort(std::string command, unsigned short& value)
{
	bool ok = false;
	
	if (command.size() == 2) 
	{
		value = command[1] - '0';
		ok = true;
	}
	
	return ok;
}