#include "ClientConnection.h"

ClientConnection::ClientConnection(unsigned long ID, ServerSocket& socket)
{
	this->ID = ID;
	this->socket = socket;
	this->state = true;
	std::thread(ReadThread, this).detach();
}


ClientConnection::~ClientConnection() { }


void ClientConnection::Write (std::string msg)
{
	this->socket << msg;
}


void ClientConnection::OnClientMessage(std::string msg)
{
	this->clientMessage.dispatchEvent(0);
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