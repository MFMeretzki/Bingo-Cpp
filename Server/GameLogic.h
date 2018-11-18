#ifndef GameLogic_class
#define GameLogic_class

#include "EventDispatcher.h"
#include "Listener.h"
#include "WaitNewGameState.h"
#include "GameStartedState.h"
#include "ClientData.h"
#include "GameState.h"
#include <string>
#include <map>


class GameLogic : public Listener
{
public:
	
	static const unsigned short CARD_PRICE;
    static const unsigned short LINE_REWARD;
    static const unsigned short BINGO_REWARD;
	
	GameLogic(std::map<unsigned long, ClientData> *clients);
	~GameLogic();
	
	void ProcessCommand (ClientData *client, std::string command);
	void ClientConnect (ClientData *client);
	bool ClientDisconnect (ClientData *client);
	
	friend void OnChangeGameState (Listener *l, int value);

private:
	
	GameState *gameState;
    std::map<unsigned long, GameState::Player> *players;
    std::map<unsigned long, ClientData> *clients;
	EventDispatcher::Pair *lPair;
	
};


void OnChangeGameState (Listener *l, int value)
{
	GameLogic *gl = static_cast<GameLogic*>(l);
	
	gl->gameState->ChangeState.removeListener(gl->lPair);
	
	switch (gl->gameState->actualState)
	{
		case GameState::State::WAIT_NEW_GAME:
		{
			delete gl->gameState;
			gl->gameState = new WaitNewGameState(gl->players,gl->clients);
			gl->gameState->ChangeState.addListener(gl->lPair);
			break;
		}
		case GameState::State::GAME_STARTED:
		{
			delete gl->gameState;
			gl->gameState = new GameStartedState(gl->players,gl->clients);
			gl->gameState->ChangeState.addListener(gl->lPair);
			break;
		}
		default:
		{
			break;
		}
	}
}


#endif