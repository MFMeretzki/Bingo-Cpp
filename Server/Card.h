#ifndef Card_class
#define Card_class

#include <string>
#include <list>
#include <algorithm>


class Card
{
public:
	Card(unsigned short values[15]);
	~Card();
	unsigned short EvaluateBall (unsigned short ball);
	std::string StringCode ();
	static std::string SStringCode (unsigned short values[15]);
	
	unsigned short ID;
	unsigned short values[15];
	std::list<unsigned short> notMarked[3];
	std::list<unsigned short> marked[3];
	bool completeLine[3];
	
private:
	
	static std::string FormatValue(unsigned short);
	
};


#endif