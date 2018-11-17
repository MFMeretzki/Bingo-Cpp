#ifndef ClientConnection_class
#define ClientConnection_class

#include "ServerSocket.h"
#include "EventDispatcher.h"
#include "ConcurrentOutput.h"
#include <iostream>
#include <thread>
#include <string>


class ClientConnection {
	
public:
	ClientConnection(unsigned long ID, ServerSocket& socket);
	~ClientConnection();
	void Write (std::string msg);
	
	unsigned long ID;
	
private:
	static void ReadThread(ClientConnection *client);
	void OnClientMessage(std::string msg);
	
	ServerSocket socket;
	EventDispatcher clientMessage;
	bool state;
	
	
};


#endif