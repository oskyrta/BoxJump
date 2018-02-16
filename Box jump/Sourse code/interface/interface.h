#pragma once
////////////////////////////////////////////////
// Include
#include "eventSystem\eventListener.h"

////////////////////////////////////////////////
// Forvard declaration
class InterfaceWindow;
class EventController;
class DataManager;
class Render;
class Game;
class UISprite;
enum HeroSkin;

////////////////////////////////////////////////
// Enum
enum MenuType
{
	MenuType_MainMenu,
	MenuType_InGameMenu,
	MenuType_PauseMenu,
	MenuType_CustomizeMenu,
	MenuType_ScoreMenu,
	MenuType_StatisticWindow,

	MenuTypeCount
};

////////////////////////////////////////////////
// Class Interface
class Interface: public EventListener
{
public:
	Interface() {};
	~Interface();

	static Interface* instance()
	{
		static Interface *instance_ = new Interface();
		return instance_;
	};

	void setupInterface();
	void initialize();
	void update();
	void render();

	InterfaceWindow* getInterfaceWindow(MenuType type) { return m_menuList[type]; }

private:
	InterfaceWindow* createWindow(MenuType type);
	void changeWindow(MenuType windowType);
	void changeSprite(std::string name, int change);

	void startListeningEvents();

private:
	InterfaceWindow* m_menuList[MenuTypeCount];

	EventController* m_eventController;
	Game* m_game;
	Render* m_render;
	DataManager* m_dataManager;

	HeroSkin m_heroSkin;
	HeroSkin m_player1Skin;
	HeroSkin m_player2Skin;

	UISprite* m_heroSprite;
	UISprite* m_player1Sprite;
	UISprite* m_player2Sprite;
	UISprite* m_cloudSprite;
};