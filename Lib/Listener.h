#ifndef Listener_class
#define Listener_class

class Listener
{
	public:
		virtual void handleEvent(void (*FuncPTR)(Listener *l, int v), int value) { }
		
};

#endif