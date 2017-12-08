////////////////////////////////////////////////
// Include
#include<SFML/Graphics.hpp>
#include "eventController.h"
#include "eventListener.h"

#include <list>

////////////////////////////////////////////////
// Variable
bool LB_DownInLastFrame = false;

////////////////////////////////////////////////
// Class EventController
EventController::EventController()
{

}

void clearList(std::pair<std::string, std::list<EventListener*>*> a) { a.second->clear(); }

EventController::~EventController()
{
	std::for_each(m_listenerList.begin(), m_listenerList.end(), clearList);
	m_listenerList.clear();
}

void EventController::update()
{
	if (m_events[GameEvent_LeftButtonDown] || m_events[GameEvent_LeftButtonStay]) LB_DownInLastFrame = true;
	else LB_DownInLastFrame = false;

	clear();

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (LB_DownInLastFrame) m_events[GameEvent_LeftButtonStay] = true;
		else m_events[GameEvent_LeftButtonDown] = true;
	}
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

void EventController::addListenerToEvent(EventListener* listener, std::string eventName, std::function<void(const EventListener*)> func)
{
	ListenersMap::iterator it = m_listenerList.find(eventName);
	if (it != m_listenerList.end())
	{
		it->second->push_back(listener);
	}
	else
	{
		ListenersList *tmp = new ListenersList();
		tmp->push_back(listener);
		m_listenerList[eventName] = tmp;
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