#ifndef EventDispatcher_class
#define EventDispatcher_class

#include "Listener.h"
#include <string>
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

		struct PairString {
			Listener *l;
			void (*FuncPTR)(Listener *l,std::string value, unsigned long id);
			
			PairString(Listener *l, void (*FuncPTR)(Listener *l,std::string  command, unsigned long id))
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
		
		void addListenerString(PairString*);
		void removeListenerString(PairString*);
		virtual void dispatchEventString(std::string value,  unsigned long id);
		
	protected:
		std::list<Pair*> mListeners;
		std::list<PairString*> sListeners;
		
};

#endif