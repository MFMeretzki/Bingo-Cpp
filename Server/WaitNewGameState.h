#ifndef WaitNewGameState_class
#define WaitNewGameState_class

#include "GameState.h"
#include "ConcurrentOutput.h"
#include <list>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>


class WaitNewGameState : public GameState
{
public:
	
	WaitNewGameState (
		std::map<unsigned long, GameState::Player> *players,
		std::map<unsigned long, ClientData*> *clients
	);
	~WaitNewGameState();
	
	// Methods
	void Start();
	void ProcessCommand (ClientData *client, std::string command);
	void ClientConnect (ClientData *client);
	bool ClientDisconnect (ClientData *client);
	void SendStartingNewGameMsg ();
	
	// Members
	const unsigned short WAITING_TIMEOUT = 15;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	
	
private:
	
	// Methods
	void SendCards (ClientData *client, unsigned short nCards);
	Card GenerateCard ();
	static void StartNewGameThread (WaitNewGameState *obj);
	
	// Members
	std::mutex sm;
	std::condition_variable cm;
	
	std::list<std::string> generatedCards;
	bool alive;
	
};


#endif