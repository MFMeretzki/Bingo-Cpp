#include "Encoder.h"


std::string Encoder::EncodeBase(unsigned short commandId)
{
	std::string command;
	command += commandId + '0';
	
	return command;
}


std::string Encoder::EncodeUShort(unsigned short commandId, unsigned short value)
{
	std::string command;
	command += commandId + '0';
	command += value + '0';
	
	return command;
}


std::string Encoder::EncodeUShort3(unsigned short commandId, unsigned short value[3])
{
	std::string command;
	command += commandId + '0';
	
	for (int i=0; i<3; ++i)
	{
		command += value[i] + '0';
	}
	
	return command;
}


std::string Encoder::EncodeCards(unsigned short commandId, std::list<Card> cards)
{
	std::string command;
	command += commandId + '0';
	command += cards.size() + '0';
	
	for (auto it = cards.begin(); it != cards.end(); ++it)
	{
		command += it->StringCode();
	}
	
	return command;
}


unsigned short Encoder::CommandID (std::string command)
{
	unsigned short code = 0;
	
	if (command.size() > 0) 
		code = command[0] - '0';
	
	return code;
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