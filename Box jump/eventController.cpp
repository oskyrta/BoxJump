////////////////////////////////////////////////
// Include
#include<SFML/Graphics.hpp>
#include "eventController.h"
#include "eventListener.h"

////////////////////////////////////////////////
// Variable
bool LB_DownInLastFrame = false;

////////////////////////////////////////////////
// Class EventController
EventController::EventController()
{
	for (int i = 0; i < GameEvent_Count; i++)
	{
		m_listenerList[i] = new ListenerListObject();
	}
}

EventController::~EventController()
{
	for (int i = 0; i < GameEvent_Count; i++)
	{
		m_listenerList[i]->deleteList();
	}
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

void EventController::startEvent(GameEvents gameEvent)
{
	ListenerListObject* tmp = m_listenerList[gameEvent];
	do
	{
		if (tmp->listener)
			tmp->listener->doOnEvent(gameEvent);
		tmp = tmp->previousListener;
	} 
	while (tmp);
}

void EventController::addListenerToEvent(EventListener* listener, GameEvents gameEvent)
{
	ListenerListObject* tmp = new ListenerListObject();

	tmp->listener = listener;
	tmp->previousListener = m_listenerList[gameEvent];

	m_listenerList[gameEvent]->nextListener = tmp;
	m_listenerList[gameEvent] = tmp;
}

void EventController::deleteListenerFromEvent(EventListener* listener, GameEvents gameEvent)
{
	ListenerListObject* tmp = m_listenerList[gameEvent];
	do
	{
		if (tmp->listener == listener)
		{
			if (tmp->nextListener)
				tmp->nextListener->previousListener = tmp->previousListener;

			if (tmp->previousListener)
				tmp->previousListener->nextListener = tmp->nextListener;
			return;
		}
		tmp = tmp->previousListener;
	}
	while (tmp);
}