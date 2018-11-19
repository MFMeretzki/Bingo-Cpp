#ifndef GameLogic_class
#define GameLogic_class

#include "ClientData.h"
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include "boost/random.hpp" 
#include <ctime>



class GameLogic
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
	
	static const unsigned short CARD_PRICE;
    static const unsigned short LINE_REWARD;
    static const unsigned short BINGO_REWARD;
	
	GameLogic(std::map<unsigned long, ClientData*> *clients);
	~GameLogic();
	
	void ProcessCommand (ClientData *client, std::string command);
	void ClientConnect (ClientData *client);
	bool ClientDisconnect (ClientData *client);
	void SendToAllPlayers (std::string command);
	void SendStartingNewGameMsg ();
	
	const unsigned short WAITING_TIMEOUT = 15;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

private:
	
	void Init();
	void InitBalls();
	
	void SendCards (ClientData *client, unsigned short nCards);
	Card GenerateCard ();
	
	static void CoreThread(GameLogic *obj);
	
    std::map<unsigned long, GameLogic::Player> *players;
    std::map<unsigned long, ClientData*> *clients;
	
	State actualState;
	std::list<std::string> generatedCards;
	std::list<unsigned short> baseNumbersList;
	std::mutex stateLock;
	
	const unsigned short BALLS_TIME_RATE = 1800;
	std::queue<unsigned short> balls;
	bool lineReached;
	bool bingoReached;
	
};


#endif