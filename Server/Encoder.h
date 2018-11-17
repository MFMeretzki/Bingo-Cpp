#ifndef Encoder_class
#define Encoder_class

#include "Card.h"
#include <string>


class Encoder {
	
public:
	
	enum Command:unsigned short {
		CLIENT_CONNECTED = 1,
		GET_CARD = 2,
		STARTING_NEW_GAME = 10,
		CARDS_RESPONSE = 11,
		BALL_REVEALED = 12,
		GAME_BEGAN = 13,
		CREDIT = 14
	};
	
	static std::string EncodeBase(unsigned short commandId);
	static std::string EncodeUShort(unsigned short commandId, unsigned short value);
	static std::string EncodeUShort3(unsigned short commandId, unsigned short value[3]);
	static std::string EncodeCards(unsigned short commandId, unsigned short nCards, Card card[]);
	
	static bool DecodeUShort(std::string command, unsigned short& value);
	
};


#endif