#ifndef ClientData_class
#define ClientData_class

#include "ClientConnection.h"
#include "Encoder.h"
#include <string>

class ClientData{
	
public:
	ClientData(ClientConnection *client, unsigned short credit);
	~ClientData();
	bool SpendCredit(int value);
	void EarnCredit(int value);
	
	ClientConnection *client;
	
private:
	int credit;
	
};


#endif