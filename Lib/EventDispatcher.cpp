#include "EventDispatcher.h"


void EventDispatcher::addListener(Pair *p)
{
	mListeners.push_back(p);
}


void EventDispatcher::removeListener(Pair *p)
{
	bool ok = true;
	std::list<Pair*>::iterator it = mListeners.begin();
	while (it != mListeners.end() && ok)
	{
		if (*it==p)
		{
			mListeners.erase(it);
			ok = false;
		}
		else 
		{
			++it;
		}
	}
}


void EventDispatcher::dispatchEvent(int value)
{
	std::list<Pair*>::iterator it = mListeners.begin();
	while (it != mListeners.end())
	{
		(*it)->l->handleEvent((*it)->FuncPTR, value);
		++it;
	}
}


void EventDispatcher::addListenerString(PairString *s)
{
	sListeners.push_back(s);
}


void EventDispatcher::removeListenerString(PairString *s)
{
	bool ok = true;
	std::list<PairString*>::iterator it = sListeners.begin();
	while (it != sListeners.end() && ok)
	{
		if (*it==s)
		{
			sListeners.erase(it);
			ok = false;
		}
		else 
		{
			++it;
		}
	}
}


void EventDispatcher::dispatchEventString(std::string value,  unsigned long id)
{
	std::list<PairString*>::iterator it = sListeners.begin();
	while (it != sListeners.end())
	{
		(*it)->l->handleEventString((*it)->FuncPTR, value, id);
		++it;
	}
}