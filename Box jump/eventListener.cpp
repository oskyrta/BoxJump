// Include
#include "eventListener.h"

// Class EventListener
EventListener::EventListener()
{
	for (int i = 0; i < GameEvent_Count; i++)
	{
		m_functions[i] = 0;
	}
}

EventListener::~EventListener()
{
	for (int i = 0; i < GameEvent_Count; i++)
	{
		m_functions[i] = 0;
	}
}