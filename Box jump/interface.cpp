/////////////////////////////////////////////////
// Include
#include "interface.h"
#include "interfaceWindow.h"
#include "game.h"
#include "eventController.h"
#include "guiButton.h"
#include "guiText.h"
#include "guiSprite.h"
#include "settingsManager.h"
#include "utils.h"
#include "hero.h"


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
	m_heroSkin = (HeroSkin)settingsManager.p_objectsSettings->get<int>("Hero.Skin");
	m_player1Skin = (HeroSkin)settingsManager.p_objectsSettings->get<int>("Player1.Skin");
	m_player2Skin = (HeroSkin)settingsManager.p_objectsSettings->get<int>("Player2.Skin");

	startListeningEvents();
}

void Interface::startListeningEvents()
{
	m_eventController->addListenerToEvent(
		this,
		"OnStart1pGameButtonDown",
		[](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; winnerCongratulation->setString(""); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnStart2pGameButtonDown",
		[](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; winnerCongratulation->setString(""); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnStartButtonDown",
		[](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnPauseButtonDown",
		[](const EventListener* listener) { activeWindowIndex = MenuType_PauseMenu; }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnMainMenuButtonDown",
		[](const EventListener* listener) { activeWindowIndex = MenuType_MainMenu; }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnCustomizeButtonDown",
		[](const EventListener* listener) {activeWindowIndex = MenuType_CustomizeMenu; }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnGameEnd",
		[](const EventListener* listener) { activeWindowIndex = MenuType_ScoreMenu; }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRestartButtonDown",
		[](const EventListener* listener) { activeWindowIndex = MenuType_InGameMenu; }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnStatisticButtonDown",
		[](const EventListener* listener) { activeWindowIndex = MenuType_StatisticWindow; }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnFirstPlayerWin",
		[](const EventListener* listener) { winnerCongratulation->setString("First player won"); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnSecondPlayerWin",
		[](const EventListener* listener) { winnerCongratulation->setString("Second player won"); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnLeftArrowSpDown",
		[](const EventListener* listener) { ((Interface*)listener)->changeSprite("Hero", -1); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRightArrowSpDown",
		[](const EventListener* listener) { ((Interface*)listener)->changeSprite("Hero", 1); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnLeftArrowP1Down",
		[](const EventListener* listener) { ((Interface*)listener)->changeSprite("Player1", -1); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRightArrowP1Down",
		[](const EventListener* listener) { ((Interface*)listener)->changeSprite("Player1", 1); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnLeftArrowP2Down",
		[](const EventListener* listener) { ((Interface*)listener)->changeSprite("Player2", -1); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRightArrowP2Down",
		[](const EventListener* listener) { ((Interface*)listener)->changeSprite("Player2", 1); }
	);

}

void Interface::initialize()
{
	Vec2 cameraPos = m_game->getCamera()->getPosition();
	GUIText* txt = 0;
	GUIButton* btn = 0;
	InterfaceWindow* window;

	// Initialize Main menu window
	window = createWindow(MenuType_MainMenu);
	window->addButton("Start game", Vec2(100, 8), "MM_Start", "OnStart1pGameButtonDown");
	window->addButton("Start 2P game", Vec2(100, 8), "MM_2pStart", "OnStart2pGameButtonDown");
	window->addButton("Exit to desktop", Vec2(100, 8), "MM_Exit", "OnExitButtonDown");
	window->addButton("Statistic", Vec2(100, 8), "MM_Statistic", "OnStatisticButtonDown");
	window->addButton("Customize", Vec2(100, 8), "MM_Customize", "OnCustomizeButtonDown");
	txt = window->addText("Box jump", Vec2(20, 20), "MM_Title", 32);
	txt = window->addText("Special for Vitek update", Vec2(20, 20), "MM_Spc", 24);

	// Initialize in-game menu
	window = createWindow(MenuType_InGameMenu);
	txt = window->addText("^", Vec2(20, 20), "GM_FPS");
	txt->setChangedValue(m_game->getPointerToFPS());

	txt = window->addText("Score: ^", Vec2(20, 20), "GM_Score");
	txt->setChangedValue(m_game->getPointerToScore());

	btn = window->addButton("", Vec2(7, 7), "GM_Pause", "OnPauseButtonDown");
	btn->setSprite("PauseButtonSprite");

	// Initialize customize menu
	window = createWindow(MenuType_CustomizeMenu);
	window->addButton("Main menu", Vec2(100, 8), "CM_Main", "OnMainMenuButtonDown");
	window->addText("Single", Vec2(20, 20), "CM_Single", 32);
	m_heroSprite = window->addSprite(getSpriteNameBySkin(m_heroSkin), "CM_HeroSprite");

	btn = window->addButton("", Vec2(16, 16), "CM_LeftArrow", "OnLeftArrowSpDown");
	btn->setSprite("LeftArrowSprite"); 
	btn = window->addButton("", Vec2(16, 16), "CM_RightArrow", "OnRightArrowSpDown");
	btn->setSprite("RightArrowSprite");

	window->addText("Co-op", Vec2(20, 20), "CM_Coop", 32);
	window->addText("Player1", Vec2(20, 20), "CM_Player1Name", 16);
	m_player1Sprite = window->addSprite(getSpriteNameBySkin(m_player1Skin), "CM_Player1Sprite");
	btn = window->addButton("", Vec2(16, 16), "CM_LeftArrowP1", "OnLeftArrowP1Down");
	btn->setSprite("LeftArrowSprite");
	btn = window->addButton("", Vec2(16, 16), "CM_RightArrowP1", "OnRightArrowP1Down");
	btn->setSprite("RightArrowSprite");

	window->addText("Player2", Vec2(20, 20), "Player2Name", 16);
	m_player2Sprite = window->addSprite(getSpriteNameBySkin(m_player2Skin), "CM_Plauer2Sprite");
	btn = window->addButton("", Vec2(16, 16), "CM_LeftArrowP2", "OnLeftArrowP2Down");
	btn->setSprite("LeftArrowSprite");
	btn = window->addButton("", Vec2(16, 16), "CM_RightArrowP2", "OnRightArrowP2Down");
	btn->setSprite("RightArrowSprite");

	// Initialize pause menu
	window = createWindow(MenuType_PauseMenu);
	window->addButton("Continue", Vec2(100, 8), "PM_Continue","OnStartButtonDown");
	window->addButton("Main menu", Vec2(100, 8), "PM_Main", "OnMainMenuButtonDown");

	txt = window->addText("Your score: ^", Vec2(20, 20), "PM_Score");
	txt->setChangedValue(m_game->getPointerToScore());

	// Initialize score menu
	window = createWindow(MenuType_ScoreMenu);
	txt = window->addText("Your score: ^", Vec2(20, 20), "SM_Score", 32);
	txt->setChangedValue(m_game->getPointerToScore());
	winnerCongratulation = window->addText("123", Vec2(20, 20), "SM_WinnCong", 32);

	window->addButton("Try again", Vec2(100, 8), "SM_Restart", "OnRestartButtonDown");
	window->addButton("Main menu", Vec2(100, 8), "SM_Main", "OnMainMenuButtonDown");

	// Initislize statistic window
	window = createWindow(MenuType_StatisticWindow);
	window->addText("Statistic", Vec2(20, 20), "StW_Title");

	txt = window->addText("High score: ^ points", Vec2(20, 20), "Stw_HS");
	txt->setChangedValue(m_game->getPointerToMaxScore());

	txt = window->addText("Time in game: ^ minutes", Vec2(20, 20), "Stw_TimeInGame");
	txt->setChangedValue(m_game->getPointerToTimeInGame());

	window->addButton("Main menu", Vec2(100, 8), "StW_Main", "OnMainMenuButtonDown");
}

void Interface::changeWindow(MenuType windowType)
{
	m_windowsList[activeWindowIndex]->setActive(false);
	activeWindowIndex = windowType;
	m_windowsList[activeWindowIndex]->setActive(true);
}

void Interface::changeSprite(std::string name, int change)
{
	if (name == "Hero")
	{
		m_heroSkin = (HeroSkin)((m_heroSkin + HeroSkin_Count + change) % HeroSkin_Count);
		m_heroSprite->initialize(getSpriteNameBySkin(m_heroSkin));
	}

	if (name == "Player1")
	{
		m_player1Skin = (HeroSkin)((m_player1Skin + HeroSkin_Count + change) % HeroSkin_Count);
		m_player1Sprite->initialize(getSpriteNameBySkin(m_player1Skin));
	}

	if (name == "Player2")
	{
		m_player2Skin = (HeroSkin)((m_player2Skin + HeroSkin_Count + change) % HeroSkin_Count);
		m_player2Sprite->initialize(getSpriteNameBySkin(m_player2Skin));
	}
}

void Interface::update()
{
	changeWindow(activeWindowIndex);

	if (m_windowsList[activeWindowIndex]->getActive()) 
		m_windowsList[activeWindowIndex]->update();
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