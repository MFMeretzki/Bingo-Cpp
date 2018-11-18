#ifndef GameState_class
#define GameState_class

#include "EventDispatcher.h"
#include "Listener.h"
#include "ClientData.h"
#include "Card.h"
#include "boost/random.hpp" 
#include <list>
#include <map>
#include <mutex>
#include <ctime>


class GameState
{
	
public:
	
	typedef boost::uniform_int<> NumberDistribution; 
	typedef boost::mt19937 RandomNumberGenerator; 
	typedef boost::variate_generator<RandomNumberGenerator&, NumberDistribution> Generator;
	
	enum State:unsigned short { WAIT_NEW_GAME, GAME_STARTED };
	struct Player
	{
		std::list<Card> cards;
		ClientData *clientData;
		
		Player(std::list<Card> cards, ClientData *clientData)
		{
			this->cards = cards;
			this->clientData = clientData;
		}
	};
	
	~GameState();
	
	virtual void ProcessCommand (ClientData *client, std::string command) = 0;
	virtual void ClientConnect (ClientData *client) = 0;
	virtual bool ClientDisconnect (ClientData *client) = 0;
	
	void SendToAllPlayers (std::string command);
	void SendToOtherPlayers (unsigned long id, std::string command);
	
	State actualState;
	EventDispatcher ChangeState;
	
	GameState(
		std::map<unsigned long, Player> players,
		std::map<unsigned long, ClientData> *clients
	);
	
	const unsigned short MAX_NUMBER = 90;
	std::map<unsigned long, Player> players;
	std::map<unsigned long, ClientData> *clients;
	std::list<unsigned short> baseNumbersList;
	static std::mutex stateLock;
	
	
};


#endif