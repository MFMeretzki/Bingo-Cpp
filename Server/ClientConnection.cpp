#include "ClientConnection.h"

ClientConnection::ClientConnection(unsigned long ID, ServerSocket& socket)
{
	this->ID = ID;
	this->socket = socket;
	this->state = false;
}


ClientConnection::~ClientConnection() { }


void ClientConnection::StartReading()
{
	this->state = true;
	std::thread(ReadThread, this).detach();
}


void ClientConnection::Write (std::string command)
{
	this->socket << command;
}


void ClientConnection::OnClientMessage(std::string command)
{
	this->clientMessage.dispatchEventString(command, this->ID);
}


void ClientConnection::ReadThread(ClientConnection *client)
{
	std::string data;
	do {
		try {
			client->socket >> data;
		}
		catch(SocketException& e) {
			ConcurrentOutput::Write("Error: " + e.description() + "/n");
			client->state = false;
		}
		
		if (!data.empty()) {
			client->OnClientMessage(data);
		}
	}while(client->state);
}