/////////////////////////////////////////////////
// Include
#include "interface.h"
#include "interfaceWindow.h"
#include "game.h"
#include "eventController.h"
#include "guiButton.h"
#include "guiText.h"
#include "settingsManager.h"


/////////////////////////////////////////////////
// Variables
static MenuType activeWindowIndex = MenuType_MainMenu;
GUIText* winnerCongratulation = 0;

/////////////////////////////////////////////////
// Extern
extern SettingsManager settingsManager;

/////////////////////////////////////////////////
// Class Interface
Interface::Interface()
{
	for (int i = 0; i < MenuTypeCount; i++)
	{
		m_windowsList[i] = 0;
	}
}

Interface::~Interface()
{
	for (int i = 0; i < MenuTypeCount; i++)
	{
		if (m_windowsList[i] != 0)
		{
			delete m_windowsList[i];
			m_windowsList[i] = 0;
		}
	}
}

void Interface::setupInterface()
{
	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_Start1pGameButtonDown);
	m_functions[GameEvent_Start1pGameButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; winnerCongratulation->setString(""); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_Start2pGameButtonDown);
	m_functions[GameEvent_Start2pGameButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; winnerCongratulation->setString(""); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_StartButtonDown);
	m_functions[GameEvent_StartButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_PauseButtonDown);
	m_functions[GameEvent_PauseButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_PauseMenu; };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_MainMenuButtonDown);
	m_functions[GameEvent_MainMenuButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_MainMenu; };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_GameEnd);
	m_functions[GameEvent_GameEnd] = [](const EventListener* listener) { activeWindowIndex = MenuType_ScoreMenu; };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_RestartButtonDown);
	m_functions[GameEvent_RestartButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_StatisticButtonDown);
	m_functions[GameEvent_StatisticButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_StatisticWindow; };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_StatisticButtonDown);
	m_functions[GameEvent_StatisticButtonDown] = [](const EventListener* listener) { activeWindowIndex = MenuType_StatisticWindow; };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_FirstPlayerWin);
	m_functions[GameEvent_FirstPlayerWin] = [](const EventListener* listener) { winnerCongratulation->setString("First player won"); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_SecondPlayerWin);
	m_functions[GameEvent_SecondPlayerWin] = [](const EventListener* listener) { winnerCongratulation->setString("Second player won"); };
}

void Interface::initialize()
{
	Vec2 cameraPos = m_game->getCamera()->getPosition();
	GUIText* txt = 0;
	GUIButton* btn = 0;
	InterfaceWindow* window;

	// Initialize Main menu window
	window = createWindow(MenuType_MainMenu);
	window->addButton("Start game", Vec2(100, 8), "MM_Start", GameEvent_Start1pGameButtonDown);
	window->addButton("Start 2P game", Vec2(100, 8), "MM_2pStart", GameEvent_Start2pGameButtonDown);
	window->addButton("Exit to desktop", Vec2(100, 8), "MM_Exit", GameEvent_ExitButtonDown);
	window->addButton("Statistic", Vec2(100, 8), "MM_Statistic", GameEvent_StatisticButtonDown);
	window->addButton("Customize", Vec2(100, 8), "MM_Customize", GameEvent_CustomizeButtonDown);
	txt = window->addText("Box jump", Vec2(20, 20), "MM_Title", 32);

	// Initialize in-game menu
	window = createWindow(MenuType_InGameMenu);
	txt = window->addText("^", Vec2(20, 20), "GM_FPS");
	txt->setChangedValue(m_game->getPointerToFPS());

	txt = window->addText("Score: ^", Vec2(20, 20), "GM_Score");
	txt->setChangedValue(m_game->getPointerToScore());

	btn = window->addButton("", Vec2(7, 7), "GM_Pause", GameEvent_PauseButtonDown);
	btn->setRect(kPauseButtonImage);

	// Initialize customize menu
	window = createWindow(MenuType_CustomizeMenu);

	// Initialize pause menu
	window = createWindow(PauseMenu);
	window->addButton("Continue", Vec2(100, 8), "PM_Continue", GameEvent_StartButtonDown);
	window->addButton("Main menu", Vec2(100, 8), "PM_Main", GameEvent_MainMenuButtonDown);

	txt = window->addText("Your score: ^", Vec2(20, 20), "PM_Score");
	txt->setChangedValue(m_game->getPointerToScore());

	// Initialize score menu
	window = createWindow(MenuType_ScoreMenu);
	txt = window->addText("Your score: ^", Vec2(20, 20), "SM_Score", 32);
	txt->setChangedValue(m_game->getPointerToScore());
	winnerCongratulation = window->addText("123", Vec2(20, 20), "SM_WinnCong", 32);

	window->addButton("Try again", Vec2(100, 8), "SM_Restart", GameEvent_RestartButtonDown);
	window->addButton("Main menu", Vec2(100, 8), "SM_Main", GameEvent_MainMenuButtonDown);

	// Initislize statistic window
	window = createWindow(MenuType_StatisticWindow);
	window->addText("Statistic", Vec2(20, 20), "StW_Title");

	txt = window->addText("Hight score: ^ points", Vec2(20, 20), "Stw_HS");
	txt->setChangedValue(m_game->getPointerToMaxScore());

	txt = window->addText("Time in game: ^ minuts", Vec2(20, 20), "Stw_TimeInGame");
	txt->setChangedValue(m_game->getPointerToTimeInGame());

	window->addButton("Main menu", Vec2(100, 8), "StW_Main", GameEvent_MainMenuButtonDown);
}

void Interface::changeWindow(MenuType windowType)
{
	m_windowsList[activeWindowIndex]->setActive(false);
	activeWindowIndex = windowType;
	m_windowsList[activeWindowIndex]->setActive(true);
}

void Interface::update()
{
	changeWindow(activeWindowIndex);

	if (m_windowsList[activeWindowIndex]->getActive()) 
		m_windowsList[activeWindowIndex]->update();

	/*
	if (m_eventController->getEventState(GameEvent_Start2pGameButtonDown)
		||
		m_eventController->getEventState(GameEvent_Start1pGameButtonDown))
	{
		changeWindow(InGameMenu);
		winnerCongratulation->setString("");
	}

	if (m_eventController->getEventState(GameEvent_StartButtonDown))		changeWindow(InGameMenu);
	if (m_eventController->getEventState(GameEvent_PauseButtonDown))		changeWindow(PauseMenu);
	if (m_eventController->getEventState(GameEvent_MainMenuButtonDown))		changeWindow(MainMenu);
	if (m_eventController->getEventState(GameEvent_GameEnd))				changeWindow(ScoreMenu);
	if (m_eventController->getEventState(GameEvent_RestartButtonDown))		changeWindow(InGameMenu);
	if (m_eventController->getEventState(GameEvent_StatisticButtonDown))	changeWindow(StatisticWindow);
	// */
}

void Interface::render()
{
	if (m_windowsList[activeWindowIndex]->getActive()) 
		m_windowsList[activeWindowIndex]->render();
}

InterfaceWindow* Interface::createWindow(MenuType type)
{
	m_windowsList[type] = new InterfaceWindow();
	m_windowsList[type]->setActive(true);
	m_windowsList[type]->setEventController(m_eventController);
	m_windowsList[type]->setCamera(m_game->getCamera());

	return m_windowsList[type];
}