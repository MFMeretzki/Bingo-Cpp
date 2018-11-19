# include "GameStartedState.h"

	
GameStartedState::GameStartedState(
	std::map<unsigned long, GameState::Player> *players,
	std::map<unsigned long, ClientData*> *clients
) : GameState (players, clients)
{
	this->actualState = GameState::State::GAME_STARTED;
	lineReached = false;
	bingoReached = false;
	alive = true;
	
	InitBalls();
	
	std::string command = Encoder::EncodeBase(Encoder::Command::GAME_BEGAN);
	SendToAllPlayers(command);
	
	std::thread(BallRevealedThread, this).detach();
}


GameStartedState::~GameStartedState() { }


void GameStartedState::Start()
{
	std::cout << "aaaaaaaaaaaaa" << std::endl;
	cm.notify_one();
	std::cout << "bbbbbbbbbbbbb" << std::endl;
}


void GameStartedState::InitBalls()
{
	while(!balls.empty())
		balls.pop();
	
	std::vector<unsigned short> v(baseNumbersList.begin(), baseNumbersList.end());
	
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle ( v.begin(), v.end(), gen);
	
	for (auto it = baseNumbersList.begin(); it != baseNumbersList.end(); ++it)
	{
		balls.push(*it);
	}
}


void GameStartedState::ProcessCommand (ClientData *client, std::string command) { }


void GameStartedState::ClientConnect (ClientData *client) { }


bool GameStartedState::ClientDisconnect (ClientData *client)
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


void GameStartedState::BallRevealedThread (GameStartedState *obj)
{
	std::unique_lock<std::mutex> lock(obj->sm);
	obj->cm.wait(lock);
	std::cout << "STARTING_NEW_GAME" << std::endl;
	
	do
	{
		if (obj->bingoReached || obj->balls.size() == 0)
		{
			obj->actualState = GameState::State::WAIT_NEW_GAME;
			obj->ChangeState.dispatchEvent((int)obj->actualState);
			obj->cm.wait(lock);
		}
		else
		{
			std::list<Player> winners;
			unsigned short ball = obj->balls.front();
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
				vector[3] = ball;
				vector[3] = 0;
				vector[3] = bingos;
				unsigned short credit = BINGO_REWARD / winners.size();
				for (auto it = winners.begin(); it != winners.end(); ++it)
				{
					it->clientData->EarnCredit(credit);
				}
			}
			else if(lines > 0 && !obj->lineReached && winners.size() > 0)
			{
                obj->lineReached = true;
				vector[3] = ball;
				vector[3] = lines;
				vector[3] = 0;
				unsigned short credit = LINE_REWARD / winners.size();
				for (auto it = winners.begin(); it != winners.end(); ++it)
				{
					it->clientData->EarnCredit(credit);
				}
			}
			else
			{
				vector[3] = ball;
				vector[3] = 0;
				vector[3] = 0;
			}
			
			std::string command = Encoder::EncodeUShort3(Encoder::Command::BALL_REVEALED, vector);
			obj->SendToAllPlayers(command);
			std::chrono::seconds seconds (2);
			std::this_thread::sleep_for(seconds);
			std::cout << "STARTING_NEW_GAME" << std::endl;
		}
	}
	while (true);
}

