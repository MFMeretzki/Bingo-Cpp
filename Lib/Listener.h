#ifndef Listener_class
#define Listener_class

#include <string>

class Listener
{
	public:
		virtual ~Listener() { }
		
		virtual void handleEvent(void (*FuncPTR)(Listener *l, int v), int value) = 0;
		virtual void handleEventString(void (*FuncPTR)(Listener *l, std::string v,  unsigned long id), std::string value,  unsigned long id) = 0;
		
};

#endif