////////////////////////////////////////////////
// Include
#include <SFML/Graphics.hpp>
#include "eventSystem/eventController.h"
#include "eventSystem/eventListener.h"

#include <list>

////////////////////////////////////////////////
// Variable
bool LB_DownInLastFrame = false;

////////////////////////////////////////////////
// Class EventController

void clearList(std::pair<std::string, std::list<EventListener*>*> a) { a.second->clear(); }

EventController::~EventController()
{
	for_each(m_listenerList.begin(), m_listenerList.end(), clearList);
	m_listenerList.clear();
}

void EventController::update()
{

}

void EventController::startEvent(std::string eventName)
{
	ListenersMap::iterator it = m_listenerList.find(eventName);
	if (it != m_listenerList.end())
	{
		for (ListenersList::iterator i = it->second->begin(); i != it->second->end(); ++i)
		{
			(*i)->doOnEvent(eventName);
		}
	}
}

void EventController::addListenerToEvent(
	EventListener* listener, 
	std::string eventName, 
	std::function<void(const EventListener*)> func)
{
	ListenersMap::iterator it = m_listenerList.find(eventName);
	if (it != m_listenerList.end())
	{
		it->second->push_back(listener);
	}
	else
	{
		m_listenerList[eventName] = new ListenersList();
		m_listenerList[eventName]->push_back(listener);
	}
	listener->setFunction(eventName, func);
}

//void EventController::deleteListenerFromEvent(EventListener* listener, GameEvents gameEvent)
//{
//	ListenerListObject* tmp = m_listenerList_old[gameEvent];
//	do
//	{
//		if (tmp->listener == listener)
//		{
//			if (tmp->nextListener)
//				tmp->nextListener->previousListener = tmp->previousListener;
//
//			if (tmp->previousListener)
//				tmp->previousListener->nextListener = tmp->nextListener;
//			return;
//		}
//		tmp = tmp->previousListener;
//	}
//	while (tmp);
//}