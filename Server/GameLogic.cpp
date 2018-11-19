#include "GameLogic.h"


const unsigned short GameLogic::CARD_PRICE = 5;
const unsigned short GameLogic::LINE_REWARD = 10;
const unsigned short GameLogic::BINGO_REWARD = 20;

GameLogic::GameLogic(std::map<unsigned long, ClientData*> *clients)
{
	unsigned short i;
	for (i=1; i<=90; ++i)
	{
		baseNumbersList.push_back(i);
	}
	
	this->clients = clients;
	this->players = new std::map<unsigned long, Player>();
	
	std::thread(CoreThread, this).detach();
}


GameLogic::~GameLogic()
{
	delete players;
}


void GameLogic::Init()
{
	this->players->clear();
	
	lineReached = false;
	bingoReached = false;
	
	InitBalls();
}


void GameLogic::InitBalls()
{
	while(!balls.empty())
		balls.pop();
	
	std::vector<unsigned short> v(baseNumbersList.begin(), baseNumbersList.end());
	
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle ( v.begin(), v.end(), gen);
	
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		balls.push(*it);
	}
}


void GameLogic::ProcessCommand (ClientData *client, std::string command)
{
	if (actualState == State::WAIT_NEW_GAME)
	{
		unsigned short commandID = Encoder::CommandID(command);
		if (commandID != 0)
		{
			this->stateLock.lock();
			{
				switch (commandID)
				{
					case Encoder::Command::CLIENT_CONNECTED:
					{
						auto now = std::chrono::high_resolution_clock::now();
						std::chrono::duration<float> elapsed = now-startTime;
						std::string sCommand = Encoder::EncodeUShort(
							Encoder::Command::STARTING_NEW_GAME,
							(unsigned short)elapsed.count()
						);
						client->client->Write(sCommand);
						break;
					}
					case Encoder::Command::GET_CARD:
					{
						unsigned short value;
						if (Encoder::DecodeUShort(command, value))
						{
							SendCards(client, value);
						}
						break;
					}
					default:
					{
						break;
					}
				}
			}
			this->stateLock.unlock();
		}
	}
}


void GameLogic::ClientConnect (ClientData *client)
{
	if (actualState == State::WAIT_NEW_GAME)
	{
		this->stateLock.lock();
		{
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = now-startTime;
			unsigned short remainingTime = WAITING_TIMEOUT - (unsigned short)elapsed.count();
				
			std::string command = Encoder::EncodeUShort (
				Encoder::Command::STARTING_NEW_GAME,
				remainingTime
			);
			client->client->Write(command);
		}
		this->stateLock.unlock();
	}
}


bool GameLogic::ClientDisconnect (ClientData *client)
{
	bool ok = true;
	
	auto it = this->players->begin();
	while (it != this->players->end() && ok)
	{
		if (it->first == client->client->ID)
		{
			this->stateLock.lock();
			this->players->erase(it);
			this->stateLock.unlock();
			ok = false;
		}
		else 
		{
			++it;
		}
	}

	return true;
}


void GameLogic::SendToAllPlayers (std::string command)
{
	for (auto it = this->players->begin(); it != this->players->end(); ++it)
	{
		it->second.clientData->client->Write(command);
	}
}


void GameLogic::SendStartingNewGameMsg ()
{
	std::string command = Encoder::EncodeUShort (
		Encoder::Command::STARTING_NEW_GAME,
		WAITING_TIMEOUT
	);
	
	this->stateLock.lock();
	{
		for (auto it = this->clients->begin(); it!=this->clients->end(); ++it)
		{
			it->second->client->Write(command);
		}
	}
	this->stateLock.unlock();
}


void GameLogic::SendCards (ClientData *client, unsigned short nCards)
{
	unsigned short n = nCards;
	if (n > 4)
		n = 4;
	
	
	//this->stateLock.lock();
	{
		if (client->SpendCredit(5 * n))
		{
			std::list<Card> cards;
			for (int i=0; i<n; ++i)
			{
				cards.push_back(GenerateCard());
			}
			
			Player player(cards,client);
			players->insert(std::pair<unsigned long, Player>(client->client->ID, player));
			std::string command = Encoder::EncodeCards(Encoder::Command::CARDS_RESPONSE, cards);
			client->client->Write(command);
		}
	}
	//this->stateLock.unlock();
}


Card GameLogic::GenerateCard ()
{
	bool ok = false;
	unsigned short cardData[15];
	do
	{
		std::vector<unsigned short> vec(baseNumbersList.begin(), baseNumbersList.end());
		std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
		std::shuffle ( vec.begin(), vec.end(), gen);
		
		for (int i=0; i<15; ++i)
		{
				cardData[i] =  vec[i];
		}
		
		std::string stringCode = Card::SStringCode(cardData);
		if (std::find(generatedCards.begin(), generatedCards.end(), stringCode) == generatedCards.end())
		{
			ok = true;
			generatedCards.push_back(stringCode);
		}

	} while (!ok);
		
	return Card(cardData);
}


void GameLogic::CoreThread(GameLogic *obj)
{
	do
	{
		obj->Init();
		bool ok1 = true;
		do {
			std::cout << "STARTING_NEW_GAME" << std::endl;
			if (obj->players->size() == 0)
			{
				obj->startTime = std::chrono::high_resolution_clock::now();
				obj->SendStartingNewGameMsg();
				std::chrono::seconds seconds (obj->WAITING_TIMEOUT);
				std::this_thread::sleep_for(seconds);
			}
			else
			{
				ok1 = false;
			}
			
			
		} while(ok1);
		
		
		bool ok2 = true;
		std::string command = Encoder::EncodeBase(Encoder::Command::GAME_BEGAN);
		obj->SendToAllPlayers(command);
		do
		{
			if (obj->bingoReached || obj->balls.size() == 0)
			{
				ok2 = false;
			}
			else
			{
				std::list<Player> winners;
				unsigned short ball = obj->balls.front();
			std::cout << "BALL :" << ball << std::endl;
				obj->balls.pop();
				
				unsigned short lines(0), bingos(0), result;
				for (auto it = obj->players->begin(); it != obj->players->end(); ++it)
				{
					for(auto it2 = it->second.cards.begin(); it2 != it->second.cards.end(); ++it2)
					{
						result = it2->EvaluateBall(ball);
						
						switch (result)
						{
							case 1:
								winners.push_back(it->second);
								lines++;
								break;
							case 2:
								winners.push_back(it->second);
								bingos++;
								break;
							default:
								break;
						}
					}
				}
				
				unsigned short vector[3];
				if (bingos > 0 && !obj->bingoReached && winners.size() > 0)
				{
					obj->bingoReached = true;
					vector[0] = ball;
					vector[1] = 0;
					vector[2] = bingos;
					unsigned short credit = BINGO_REWARD / winners.size();
					for (auto it = winners.begin(); it != winners.end(); ++it)
					{
						it->clientData->EarnCredit(credit);
					}
				}
				else if(lines > 0 && !obj->lineReached && winners.size() > 0)
				{
					obj->lineReached = true;
					vector[0] = ball;
					vector[1] = lines;
					vector[2] = 0;
					unsigned short credit = LINE_REWARD / winners.size();
					for (auto it = winners.begin(); it != winners.end(); ++it)
					{
						it->clientData->EarnCredit(credit);
					}
				}
				else
				{
					vector[0] = ball;
					vector[1] = 0;
					vector[2] = 0;
				}
				
				std::string command = Encoder::EncodeUShort3(Encoder::Command::BALL_REVEALED, vector);
				obj->SendToAllPlayers(command);
				std::chrono::seconds seconds (2);
				std::this_thread::sleep_for(seconds);
			}
		}
		while (ok2);
	}
	while (true);
}