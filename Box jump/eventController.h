#pragma once
////////////////////////////////////////////////
// Include

////////////////////////////////////////////////
// Forvard declaration
class EventListener;

////////////////////////////////////////////////
// Enum
enum GameEvents
{
	GameEvent_None,

	GameEvent_StartButtonDown,
	GameEvent_PauseButtonDown,
	GameEvent_Start1pGameButtonDown,
	GameEvent_Start2pGameButtonDown,
	GameEvent_MainMenuButtonDown,
	GameEvent_ExitButtonDown,
	GameEvent_StatisticButtonDown,
	GameEvent_RestartButtonDown,

	GameEvent_FirstPlayerWin,
	GameEvent_SecondPlayerWin,

	GameEvent_LeftButtonDown,
	GameEvent_LeftButtonStay,

	GameEvent_GameEnd,

	GameEvent_Count
};

////////////////////////////////////////////////////
// struct ListenerListObject
struct ListenerListObject
{
	ListenerListObject();
	void deleteList();

	ListenerListObject* previousListener;
	ListenerListObject* nextListener;
	EventListener* listener;
};

ListenerListObject::ListenerListObject()
{
	previousListener = 0;
	nextListener = 0;
	listener = 0;
}

void ListenerListObject::deleteList()
{
	if (previousListener != 0)
		previousListener->deleteList();
	delete this;
}

////////////////////////////////////////////////
// Class EventController
class EventController 
{
public:
	EventController();
	~EventController();

	void clear() { for(int i = 0; i < 16; i++) m_events[i] = false; };
	void update();

	bool getEventState(GameEvents gameEvent) { return m_events[gameEvent]; };
	void startEvent(GameEvents gameEvent);

	void addListenerToEvent(EventListener* listener, GameEvents gameEvent);
	void deleteListenerFromEvent(EventListener* listener, GameEvents gameEvent);

private:
	bool m_events[GameEvent_Count];
	ListenerListObject* m_listenerList[GameEvent_Count];
};