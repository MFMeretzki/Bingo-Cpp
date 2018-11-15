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