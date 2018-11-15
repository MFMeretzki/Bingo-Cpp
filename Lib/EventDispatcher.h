#ifndef EventDispatcher_class
#define EventDispatcher_class

#include "Listener.h"
#include <list>

struct Pair {
	Listener *l;
	void (*FuncPTR)(Listener *l,int value);
};


class EventDispatcher {
	
	public:
		EventDispatcher() { }
		virtual ~EventDispatcher() { }
		void addListener(Pair*);
		void removeListener(Pair*);
		virtual void dispatchEvent(int value);
		
	protected:
		std::list<Pair*> mListeners;
		
};

#endif