#include "GameState.h"


GameState::GameState(std::map<unsigned long, Player> *players, std::map<unsigned long, ClientData> *clients)
{
	this->players = players;
	this->clients = clients;
	
	unsigned short i;
	for (i=1; i<=MAX_NUMBER; ++i)
	{
		baseNumbersList.push_back(i);
	}
}


GameState::~GameState() { }


void GameState::SendToAllPlayers (std::string command)
{
	for (auto it = this->players->begin(); it != this->players->end(); ++it)
	{
		it->second.clientData->client->Write(command);
	}
}


void GameState::SendToOtherPlayers (unsigned long id, std::string command)
{
	for (auto it = this->players->begin(); it != this->players->end(); ++it)
	{
		if (it->first != id)
			it->second.clientData->client->Write(command);
	}
}