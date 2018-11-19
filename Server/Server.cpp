#include "Server.h"


Server::Server()
{
	clients = new std::map<unsigned long, ClientData*>();
	this->lPair = new EventDispatcher::PairString(this, OnProcessCommand);
	
	alive = false;
}


Server::~Server()
{
	alive = false;
	delete clients;
	delete lPair;
	delete gameLogic;
}


void Server::Start()
{
	//std::thread(ListenerThread, this).detach();
	gameLogic = new GameLogic(clients);
	ServerSocket *serverSocket;
	
	try {
		serverSocket = new ServerSocket(25564);
		this->alive = true;
	}
	catch(SocketException& e)
	{
		ConcurrentOutput::Write("Error: " + e.description());
	}
	
	while(this->alive)
	{
		try{
			ServerSocket *clientSocket = new ServerSocket();
			serverSocket->accept(*clientSocket);
			
			unsigned long ID = this->GetNextID();
			ClientConnection *clientConnection = new ClientConnection(ID, *clientSocket);
			ClientData *clientData = new ClientData(clientConnection, 50);
			this->clients->insert(std::pair<unsigned long, ClientData*>(ID,clientData));
			
			clientConnection->clientMessage.addListenerString(this->lPair);
			clientConnection->StartReading();
			
			this->gameLogic->ClientConnect (clientData);
		}
		catch(SocketException& e)
		{
			ConcurrentOutput::Write("Error: " + e.description());
		}
	}
}


void Server::ListenerThread(Server *server)
{
	ServerSocket *serverSocket;
	try {
		serverSocket = new ServerSocket(25564);
		server->alive = true;
	}
	catch(SocketException& e)
	{
		ConcurrentOutput::Write("Error: " + e.description());
	}
	
	while(server->alive)
	{
		try{
			ServerSocket *clientSocket = new ServerSocket();
			serverSocket->accept(*clientSocket);
			
			unsigned long ID = server->GetNextID();
			ClientConnection *clientConnection = new ClientConnection(ID, *clientSocket);
			ClientData *clientData = new ClientData(clientConnection, 50);
			server->clients->insert(std::pair<unsigned long, ClientData*>(ID,clientData));
			
			clientConnection->clientMessage.addListenerString(server->lPair);
			clientConnection->StartReading();
			
			server->gameLogic->ClientConnect (clientData);
		}
		catch(SocketException& e)
		{
			ConcurrentOutput::Write("Error: " + e.description());
		}
	}
}


unsigned long Server::GetNextID ()
{
	return ++nextID;
}


void Server::handleEvent(void (*FuncPTR)(Listener *l, int v), int value)
{
	FuncPTR(this, value);
}


void Server::handleEventString(
	void (*FuncPTR)(Listener *l, std::string v,
	unsigned long id), std::string value,
	unsigned long id
)
{
	FuncPTR(this, value, id);
}


void OnProcessCommand (Listener *l, std::string command, unsigned long ID)
{
	Server *server = static_cast<Server*>(l);
	server->gameLogic->ProcessCommand (server->clients->at(ID), command);
};