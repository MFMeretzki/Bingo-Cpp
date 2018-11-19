#ifndef GameState_class
#define GameState_class

#include "EventDispatcher.h"
#include "Listener.h"
#include "ClientData.h"
#include "Card.h"
#include <list>
#include <map>
#include <thread>
#include <mutex>
#include "boost/random.hpp" 
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
	
	GameState(
		std::map<unsigned long, Player> *players,
		std::map<unsigned long, ClientData*> *clients
	);
	virtual ~GameState() = 0;
	
	virtual void Start () = 0;
	virtual void ProcessCommand (ClientData *client, std::string command) = 0;
	virtual void ClientConnect (ClientData *client) = 0;
	virtual bool ClientDisconnect (ClientData *client) = 0;
	
	void SendToAllPlayers (std::string command);
	void SendToOtherPlayers (unsigned long id, std::string command);
	
	
	State actualState;
	EventDispatcher ChangeState;
	
	const unsigned short MAX_NUMBER = 90;
    static const unsigned short LINE_REWARD;
    static const unsigned short BINGO_REWARD;
	std::map<unsigned long, Player> *players;
	std::map<unsigned long, ClientData*> *clients;
	std::list<unsigned short> baseNumbersList;
	std::mutex stateLock;
	
	
};


#endif