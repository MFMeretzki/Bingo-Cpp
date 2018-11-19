#ifndef GameStartedState_class
#define GameStartedState_class

#include "GameState.h"
#include "ConcurrentOutput.h"
#include <queue>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

class GameStartedState : public GameState
{
public:
	
	GameStartedState(
		std::map<unsigned long, GameState::Player> *players,
		std::map<unsigned long, ClientData*> *clients
	);
	~GameStartedState();
	
	
private:
	
	void Start ();
	void InitBalls();
	void ProcessCommand (ClientData *client, std::string command);
	void ClientConnect (ClientData *client);
	bool ClientDisconnect (ClientData *client);
	static void BallRevealedThread (GameStartedState *obj);
	
	const unsigned short BALLS_TIME_RATE = 1800;
	std::queue<unsigned short> balls;
	bool lineReached;
	bool bingoReached;
	bool alive;
	
	std::mutex sm;
	std::condition_variable cm;
};


#endif