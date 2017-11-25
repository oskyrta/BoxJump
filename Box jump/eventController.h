#pragma once
////////////////////////////////////////////////
// Enum
enum GameEvents
{
	GameEvent_None,
	GameEvent_StartButtonDown,
	GameEvent_Start1pGameButtonDown,
	GameEvent_Start2pGameButtonDown,
	GameEvent_PauseButtonDown,
	GameEvent_MainMenuButtonDown,
	GameEvent_ExitButtonDown,
	GameEvent_StatisticButtonDown,
	GameEvent_LeftButtonDown,
	GameEvent_LeftButtonStay,
	GameEvent_RestartButtonDown,
	GameEvent_GameEnd
};

////////////////////////////////////////////////
// Class EventController
class EventController 
{
public:
	//EventController();
	//~EventController();

	void clear() { for(int i = 0; i < 16; i++) m_events[i] = false; };
	void update();

	bool getEventState(GameEvents gameEvent) { return m_events[gameEvent]; };
	void startEvent(GameEvents gameEvent) { m_events[gameEvent] = true; };

private:
	bool m_events[16];
};