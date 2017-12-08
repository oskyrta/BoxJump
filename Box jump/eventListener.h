#pragma once
//////////////////////////////////////////////////
// Include
#include <iostream>
#include <functional>
#include <map>
#include "eventController.h"

//////////////////////////////////////////////////
// Class EventListener
class EventListener
{
public:
	EventListener();
	~EventListener();

	void doOnEvent(std::string eventName) { m_functions.find(eventName)->second(this); }
	void setFunction(std::string eventName, std::function<void(const EventListener*)> func) { m_functions[eventName] = func; }

protected:
	std::map<std::string, std::function<void(const EventListener*)>> m_functions;
};