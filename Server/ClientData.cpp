#include "ClientData.h"


ClientData::ClientData(ClientConnection *client, unsigned short credit) {
	
	this->client = client;
	this->credit = credit;
	
	std::string command = Encoder::EncodeUShort(Encoder::Command::CREDIT, credit);
	client->Write(command);
	
}


ClientData::~ClientData() { }


bool ClientData::SpendCredit(int value) {
	
	bool ok = false;
	
	if (value <= this->credit)
	{
		 this->credit -= value;

		std::string command = Encoder::EncodeUShort(Encoder::Command::CREDIT, credit);
		client->Write(command);
		ok = true;
	}
	
	return ok;
}


void ClientData::EarnCredit(int value) {
	credit += value;

	std::string command = Encoder::EncodeUShort(Encoder::Command::CREDIT, credit);
	client->Write(command);
}