#pragma once
////////////////////////////////////////////////
// Include
#include "eventListener.h"

////////////////////////////////////////////////
// Forvard declaration
class InterfaceWindow;
class EventController;
class Game;
class GUISprite;

enum HeroSkin;

////////////////////////////////////////////////
// Enum
enum MenuType
{
	MenuType_MainMenu,
	MenuType_InGameMenu,
	MenuType_PauseMenu,
	MenuType_ScoreMenu,
	MenuType_StatisticWindow,
	MenuType_CustomizeMenu,

	MenuTypeCount
};

////////////////////////////////////////////////
// Class Interface
class Interface: public EventListener
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
	void changeSprite(std::string name, int change);

private:
	InterfaceWindow* createWindow(MenuType type);
	void changeWindow(MenuType windowType);

	void startListeningEvents();

private:
	InterfaceWindow* m_windowsList[10];

	EventController* m_eventController;

	Game* m_game;	
	
	HeroSkin m_heroSkin;
	HeroSkin m_player1Skin;
	HeroSkin m_player2Skin;

	GUISprite* m_heroSprite;
	GUISprite* m_player1Sprite;
	GUISprite* m_player2Sprite;
};