#ifndef Server_class
#define Server_class

#include "EventDispatcher.h"
#include "Listener.h"
#include "GameLogic.h"
#include <map>
#include <string>


class Server : public Listener
{
public:
	Server();
	~Server();
	
	void Start();
	static void ListenerThread(Server *);
	
	static const unsigned short BASE_CREDIT = 50;
	static const int PORT = 25564;
	GameLogic *gameLogic;
	std::map<unsigned long, ClientData*> *clients;
	
	virtual void handleEvent(void (*FuncPTR)(Listener *l, int v), int value);
	virtual void handleEventString(void (*FuncPTR)(Listener *l, std::string v,  unsigned long id), std::string value,  unsigned long id);
	
	friend void OnProcessCommand (Listener *l, std::string value, unsigned long ID);
	
private:
	
	unsigned long GetNextID ();
	
	unsigned long nextID;
	bool alive;
	EventDispatcher::PairString *lPair;
	
};


void OnProcessCommand (Listener *l, std::string value, unsigned long ID);


#endif