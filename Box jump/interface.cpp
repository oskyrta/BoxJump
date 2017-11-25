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
int activeWindowIndex = MainMenu;


extern SettingsManager settingsManager;

/////////////////////////////////////////////////
// Class Interface
Interface::Interface()
{
	for (int i = 0; i < WindowsTypeCount; i++)
	{
		m_windowsList[i] = 0;
	}
}

Interface::~Interface()
{
	for (int i = 0; i < WindowsTypeCount; i++)
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

}

void Interface::initialize()
{
	Vec2 cameraPos = m_game->getCamera()->getPosition();
	GUIText* txt = 0;
	GUIButton* btn = 0;
	InterfaceWindow* window;

	// Initialize Main menu window
	window = createWindow(MainMenu);
	window->addButton("Start game", Vec2(100, 8), "MM_Start", GameEvent_Start1pGameButtonDown);
	window->addButton("Start 2P game", Vec2(100, 8), "MM_2pStart", GameEvent_Start2pGameButtonDown);
	window->addButton("Exit to desktop", Vec2(100, 8), "MM_Exit", GameEvent_ExitButtonDown);
	window->addButton("Statistic", Vec2(100, 8), "MM_Statistic", GameEvent_StatisticButtonDown);
	txt = window->addText("Box jump", Vec2(20, 20), "MM_Title", 32);


	// Initialize in-game menu
	window = createWindow(InGameMenu);
	txt = window->addText("^", Vec2(20, 20), "GM_FPS");
	txt->setChangedValue(m_game->getPointerToFPS());

	txt = window->addText("Score: ^", Vec2(20, 20), "GM_Score");
	txt->setChangedValue(m_game->getPointerToScore());

	btn = window->addButton("", Vec2(7, 7), "GM_Pause", GameEvent_PauseButtonDown);
	btn->setRect(kPauseButtonImage);

	// Initialize pause menu
	window = createWindow(PauseMenu);
	window->addButton("Continue", Vec2(100, 8), "PM_Continue", GameEvent_StartButtonDown);
	window->addButton("Main menu", Vec2(100, 8), "PM_Main", GameEvent_MainMenuButtonDown);

	txt = window->addText("Your score: ^", Vec2(20, 20), "PM_Score");
	txt->setChangedValue(m_game->getPointerToScore());

	// Initialize score menu
	window = createWindow(ScoreMenu);
	txt = window->addText("Your score: ^", Vec2(20, 20), "SM_Score", 32);
	txt->setChangedValue(m_game->getPointerToScore());
	window->addButton("Try again", Vec2(100, 8), "SM_Restart", GameEvent_RestartButtonDown);
	window->addButton("Main menu", Vec2(100, 8), "SM_Main", GameEvent_MainMenuButtonDown);

	// Initislize statistic window
	window = createWindow(StatisticWindow);
	window->addText("Statistic", Vec2(20, 20), "StW_Title");

	txt = window->addText("Hight score: ^ points", Vec2(20, 20), "Stw_HS");
	txt->setChangedValue(m_game->getPointerToMaxScore());

	txt = window->addText("Time in game: ^ minuts", Vec2(20, 20), "Stw_TimeInGame");
	txt->setChangedValue(m_game->getPointerToTimeInGame());

	window->addButton("Main menu", Vec2(100, 8), "StW_Main", GameEvent_MainMenuButtonDown);
}

void Interface::changeWindow(WindowsType windowType)
{
	m_windowsList[activeWindowIndex]->setActive(false);
	activeWindowIndex = windowType;
	m_windowsList[activeWindowIndex]->setActive(true);
}

void Interface::update()
{
	if (m_windowsList[activeWindowIndex]->getActive()) 
		m_windowsList[activeWindowIndex]->update();

	if(m_game->getGameEnded() && activeWindowIndex == InGameMenu)
		m_eventController->startEvent(GameEvent_GameEnd);

	if (m_eventController->getEventState(GameEvent_StartButtonDown)) changeWindow(InGameMenu);
	if (m_eventController->getEventState(GameEvent_Start2pGameButtonDown) || m_eventController->getEventState(GameEvent_Start1pGameButtonDown)) changeWindow(InGameMenu);
	if (m_eventController->getEventState(GameEvent_PauseButtonDown)) changeWindow(PauseMenu);
	if (m_eventController->getEventState(GameEvent_MainMenuButtonDown)) changeWindow(MainMenu);
	if (m_eventController->getEventState(GameEvent_GameEnd)) changeWindow(ScoreMenu);
	if (m_eventController->getEventState(GameEvent_RestartButtonDown)) changeWindow(InGameMenu);
	if (m_eventController->getEventState(GameEvent_StatisticButtonDown)) changeWindow(StatisticWindow);
}

void Interface::render()
{
	if (m_windowsList[activeWindowIndex]->getActive()) 
		m_windowsList[activeWindowIndex]->render();
}

InterfaceWindow* Interface::createWindow(WindowsType type)
{
	m_windowsList[type] = new InterfaceWindow();
	m_windowsList[type]->setActive(true);
	m_windowsList[type]->setEventController(m_eventController);
	m_windowsList[type]->setCamera(m_game->getCamera());

	return m_windowsList[type];
}