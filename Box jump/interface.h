#pragma once
////////////////////////////////////////////////
// Include

////////////////////////////////////////////////
// Forvard declaration
class InterfaceWindow;
class EventController;
class Game;

////////////////////////////////////////////////
// Enum
enum WindowsType
{
	MainMenu,
	InGameMenu,
	PauseMenu,
	ScoreMenu,
	StatisticWindow,

	WindowsTypeCount
};

////////////////////////////////////////////////
// Class Interface
class Interface
{
public:
	Interface();
	~Interface();

	void setupInterface();
	void initialize();
	void update();
	void render();

	void setGame(Game* game) { m_game = game; };
	void setEventController(EventController* ec) { m_eventController = ec; };

private:
	InterfaceWindow* createWindow(WindowsType type);
	void changeWindow(WindowsType windowType);

	InterfaceWindow* m_windowsList[10];

	EventController* m_eventController;

	Game* m_game;
};