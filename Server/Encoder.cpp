#include "Encoder.h"


std::string Encoder::EncodeBase(unsigned short commandId)
{
	std::string command;
	command += std::to_string(commandId);
	
	return command;
}


std::string Encoder::EncodeUShort(unsigned short commandId, unsigned short value)
{
	std::string command;
	command += std::to_string(commandId);
	command += FormatValue (value);
	
	return command;
}


std::string Encoder::EncodeUShort3(unsigned short commandId, unsigned short value[3])
{
	std::string command;
	command += std::to_string(commandId);
	
	for (int i=0; i<3; ++i)
	{
		command += FormatValue (value[i]);
	}
	
	return command;
}


std::string Encoder::EncodeCards(unsigned short commandId, std::list<Card> cards)
{
	std::string command;
	command += std::to_string(commandId);
	command += FormatValue (cards.size());
	
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
	
	if (command.size() == 3) 
	{
		int d = command[1] - '0';
		int u = command[2] - '0';
		value = (d * 10) + u;
		ok = true;
	}
	
	return ok;
}


std::string FormatValue (unsigned short value)
{
	std::string s;
	
	if (value < 10)
		s += std::to_string(0);
	
	s += std::to_string(value);
	
	return s;
}