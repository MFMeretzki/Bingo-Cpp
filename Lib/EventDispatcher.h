#ifndef EventDispatcher_class
#define EventDispatcher_class

#include "Listener.h"
#include <list>


class EventDispatcher {
	
	public:

		struct Pair {
			Listener *l;
			void (*FuncPTR)(Listener *l,int value);
			
			Pair(Listener *l, void (*FuncPTR)(Listener *l,int value))
			{
				this->l = l;
				this->FuncPTR = FuncPTR;
			}
		};
		
		EventDispatcher() { }
		virtual ~EventDispatcher() { }
		void addListener(Pair*);
		void removeListener(Pair*);
		virtual void dispatchEvent(int value);
		
	protected:
		std::list<Pair*> mListeners;
		
};

#endif