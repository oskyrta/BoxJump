#pragma once
//////////////////////////////////////////////////
// Include
#include <iostream>
#include <functional>
#include "eventController.h"

//////////////////////////////////////////////////
// Class EventListener
class EventListener
{
public:
	EventListener();
	~EventListener();

	void doOnEvent(GameEvents gameEvent) { if(m_functions[gameEvent]) m_functions[gameEvent](this); }

protected:
	std::function<void(const EventListener*)> m_functions[GameEvent_Count];
};