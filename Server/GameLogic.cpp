#include "GameLogic.h"


const unsigned short GameLogic::CARD_PRICE = 5;
const unsigned short GameLogic::LINE_REWARD = 10;
const unsigned short GameLogic::BINGO_REWARD = 20;

GameLogic::GameLogic(std::map<unsigned long, ClientData> *clients)
{
	this->clients = clients;
	this->players = new std::map<unsigned long, GameState::Player>();
	this->lPair = new EventDispatcher::Pair(this, OnChangeGameState);

	gameState = new WaitNewGameState(players, clients);
	gameState->ChangeState.addListener(lPair);
}


GameLogic::~GameLogic()
{
	delete players;
	delete lPair;
	delete gameState;
}


void GameLogic::ProcessCommand (ClientData *client, std::string command)
{
	gameState->ProcessCommand(client, command);
}


void GameLogic::ClientConnect (ClientData *client)
{
	gameState->ClientConnect(client);
}


bool GameLogic::ClientDisconnect (ClientData *client)
{
	return gameState->ClientDisconnect(client);
}