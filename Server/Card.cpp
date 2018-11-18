#include "Card.h"


Card::Card(unsigned short values[15]) {
	
	for (int i=0; i<3; ++i)
	{
		this->completeLine[i] = false;

		for (int j=0; j<5; ++j)
		{
			this->notMarked[i].push_back(values[j + (i*5)]);
			this->values[j+(i*5)] = values[j + (i*5)];
		}
	}
}


Card::~Card() { }


unsigned short Card::EvaluateBall (unsigned short ball)
{
	unsigned short retValue = 0;        // 0 -> nothing special, 1 -> LINE!, 2 -> BINGO!!
	for (int i=0; i<3; ++i)
	{
		if(std::find(this->notMarked[i].begin(), this->notMarked[i].end(), ball) != this->notMarked[i].end())
		{
			this->notMarked[i].remove(ball);
			this->marked[i].push_back(ball);
			if (this->marked[i].size() == 5)
			{
				completeLine[i] = true;
				retValue = 1;
			}

			if (this->completeLine[0] && this->completeLine[1] && this->completeLine[2])
			{
				retValue = 2;
			}
		}
	}

	return retValue;
}


std::string Card::StringCode ()
{
	std::string code ;
	
	for (int i=0; i<15; ++i)
	{
		code += this->values[i] + '0';
	}

	return code;
}



std::string Card::SStringCode (unsigned short values[15]){
	std::string code ;
	
	for (int i=0; i<15; ++i)
	{
		code += values[i] + '0';
	}

	return code;
}