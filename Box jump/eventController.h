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
// Enum
enum GameEvents
{
	GameEvent_None,

	GameEvent_LeftButtonDown,
	GameEvent_LeftButtonStay,

	GameEvent_Count
};

////////////////////////////////////////////////
// Class EventController
class EventController 
{
public:
	EventController();
	~EventController();

	void clear() { for(int i = 0; i < GameEvent_Count; i++) m_events[i] = false; };
	void update();

	bool getEventState(GameEvents gameEvent) { return m_events[gameEvent]; };
	void startEvent(std::string eventName = "");

	void addListenerToEvent(EventListener* listener, std::string eventName = "", std::function<void(const EventListener*)> func = 0);
	//void deleteListenerFromEvent(EventListener* listener, GameEvents gameEvent);

private:
	bool m_events[GameEvent_Count];

	ListenersMap m_listenerList;
};