#include "WaitNewGameState.h"


WaitNewGameState::WaitNewGameState (
	std::map<unsigned long, GameState::Player> *players,
	std::map<unsigned long, ClientData> *clients
) : GameState (players, clients)
{
	this->alive = true;
	this->actualState = GameState::State::WAIT_NEW_GAME;
	this->players->clear();
	
	std::thread(StartNewGameThread, this).detach();
}


WaitNewGameState::~WaitNewGameState ()
{
	this->alive = false;
}


void WaitNewGameState::ProcessCommand (ClientData *client, std::string command)
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


void WaitNewGameState::ClientConnect (ClientData *client)
{
	this->stateLock.lock();
	{
		ConcurrentOutput::Write("Client connected: " + client->client->ID);
		
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = now-startTime;
		unsigned short remainingTime = 
			WaitNewGameState::WAITING_TIMEOUT - (unsigned short)elapsed.count();
			
		std::string command = Encoder::EncodeUShort (
			Encoder::Command::STARTING_NEW_GAME,
			remainingTime
		);
		client->client->Write(command);
	}
	this->stateLock.unlock();
}


bool WaitNewGameState::ClientDisconnect (ClientData *client)
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


void WaitNewGameState::SendStartingNewGameMsg ()
{
	std::string command = Encoder::EncodeUShort (
		Encoder::Command::STARTING_NEW_GAME,
		WaitNewGameState::WAITING_TIMEOUT
	);
	
	this->stateLock.lock();
	{
		for (auto it = this->clients->begin(); it!=this->clients->end(); ++it)
		{
			it->second.client->Write(command);
		}
	}
	this->stateLock.unlock();
}


void WaitNewGameState::SendCards (ClientData *client, unsigned short nCards)
{
	if (nCards > 4)
		nCards = 4;
	
	this->stateLock.lock();
	{
		if (client->SpendCredit(GameLogic::CARD_PRICE * nCards))
		{
			std::list<Card> cards;
			for (int i=0; i<nCards; ++i)
			{
				cards.push_back(GenerateCard());
			}
			
			Player player(cards,client);
			players->insert(std::pair<unsigned long, Player>(client->client->ID, player));
			
			std::string command = Encoder::EncodeCards(Encoder::Command::CARDS_RESPONSE, cards);
			client->client->Write(command);
		}
	}
	this->stateLock.unlock();
}


Card WaitNewGameState::GenerateCard ()
{
	bool ok = false;
	unsigned short cardData[15];
	do
	{
		std::list<unsigned short> list(baseNumbersList);
		for (int i=0; i<15; ++i)
		{
				NumberDistribution distribution(1, 90-i);
				RandomNumberGenerator generator;
				Generator numberGenerator(generator, distribution);
				generator.seed(std::time(0));
				
				cardData[i] = (unsigned short)numberGenerator();
		}
		
		std::string stringCode = Card::SStringCode(cardData);
		if (std::find(generatedCards.begin(), generatedCards.end(), stringCode) != generatedCards.end())
		{
			ok = true;
			generatedCards.push_back(stringCode);
		}

	} while (!ok);
		
	return Card(cardData);
}


void WaitNewGameState::StartNewGameThread (WaitNewGameState *obj)
{
	do {
		if (obj->players->size() == 0)
		{
			ConcurrentOutput::Write("WAIT_NEW_GAME");
			obj->startTime = std::chrono::high_resolution_clock::now();
			obj->SendStartingNewGameMsg();
			std::chrono::seconds seconds (obj->WAITING_TIMEOUT);
			std::this_thread::sleep_for(seconds);
		}
		else
		{
			obj->alive = false;
			obj->actualState = GameState::State::GAME_STARTED;
			
			obj->ChangeState.dispatchEvent((int)obj->actualState);
		}
		
		
	} while(obj->alive);
}