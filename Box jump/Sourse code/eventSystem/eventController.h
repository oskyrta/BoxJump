#pragma once
////////////////////////////////////////////////
// Include
#include <list>
#include <map>
#include <functional>

////////////////////////////////////////////////
// Forvard declaration
class EventListener;

////////////////////////////////////////////////
// Type define
typedef std::list<EventListener*> ListenersList;
typedef std::map<std::string, ListenersList*> ListenersMap;

////////////////////////////////////////////////
// Class EventController
class EventController 
{
public:
	EventController() {};
	~EventController();

	static EventController* instance()
	{
		static EventController *instance_ = new EventController();
		return instance_;
	};

	void update();

	void startEvent(std::string eventName);

	void addListenerToEvent(EventListener* listener, std::string eventName = "", std::function<void(const EventListener*)> func = 0);
	//void deleteListenerFromEvent(EventListener* listener, GameEvents gameEvent);

private:
	ListenersMap m_listenerList;
};