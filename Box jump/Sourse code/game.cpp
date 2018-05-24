////////////////////////////////////////////////
// Include
#include "game.h"
#include "dataManager/levelSettings.h"
#include "dataManager\dataManager.h"
#include "render\render.h"
#include "render\camera.h"

#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <Windows.h>
#include <cmath>
#include <stdlib.h>
#include <thread>
#include <chrono>

#include "tools\utils.h"
#include "tools\vec2.h"
#include "physics\collision.h"

#include "tools\pool.h"
#include "controller.h"
#include "eventSystem/eventController.h"
#include "eventSystem/eventListener.h"
#include "physics\physicsController.h"

#include "interface\interface.h"
#include "interface\interfaceWindow.h"
#include "interface\uiText.h"

#include "gameObject\hero.h"
#include "gameObject\platform.h"
#include "gameObject\cloud.h"

////////////////////////////////////////////////
// Const
const Vec2 zeroVec = Vec2();

////////////////////////////////////////////////
// Function declaration
void GetInterval(ObjectPolygon* a, Vec2 axis, float& min0, float& max0);
std::string getSpriteNameByType(GameObjectType type);
float Vec2Dot(Vec2 a, Vec2 axis);

////////////////////////////////////////////////
// Variables
float t = 0;
int frames = 0;
float distance = 0;
float lastYpos = 140;

////////////////////////////////////////////////
// Class Game

Game::Game()
{
	m_isGameEnded = false;
	
	m_isGameSessionActive = false;

	m_lastClock = 0;
	m_fps = 0;

	m_score = 0;
	m_highScore = 0;
	m_timeInGame = 0;

	m_player1 = 0;
	m_player2 = 0;
	m_platformPool = 0;
	m_controller = 0;
	m_physicsController = 0;

	m_mainCamera = 0;
	m_requiredCameraYPosition = 0;
	m_eventController = 0;
}

Game::~Game()
{
	m_dataManager->getObjectsSettgins()->put<int>("Hero.Skin", (int)m_heroSkin);
	m_dataManager->getObjectsSettgins()->put<int>("Player1.Skin", (int)m_player1Skin);
	m_dataManager->getObjectsSettgins()->put<int>("Player2.Skin", (int)m_player2Skin);

	updateStatistic();
}

void Game::setupSystem()
{
	// Set seed to random generator
	srand(time(0));

	// Set start clock
	m_lastClock = clock();

	// Initialize pools
	m_platformPool = new Pool();
	m_cloudPool = new Pool();

	m_controller = new Controller();

	m_physicsController = new PhysicsController();
	m_dataManager = DataManager::instance();
	m_eventController = EventController::instance();
	m_mainCamera = Render::instance()->getCamera("Main");

	m_timeInGame = m_dataManager->getStatistic()->get<double>("TimeInGame", 0);
	m_minutesInGame = m_timeInGame / 60;
	m_highScore = m_dataManager->getStatistic()->get<int>("MaxScore", 0);

	m_heroSkin = (HeroSkin)m_dataManager->getObjectsSettgins()->get<int>("Hero.Skin");
	m_player1Skin = (HeroSkin)m_dataManager->getObjectsSettgins()->get<int>("Player1.Skin");
	m_player2Skin = (HeroSkin)m_dataManager->getObjectsSettgins()->get<int>("Player2.Skin");

	startListeningEvents();
}

void Game::startListeningEvents()
{	
	m_eventController->addListenerToEvent(
		this,
		"OnStart1pGameButtonDown",
		[](const EventListener* listener) { ((Game*)listener)->startNew1PGame(); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnStart2pGameButtonDown",
		[](const EventListener* listener) { ((Game*)listener)->startNew2PGame(); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnStartButtonDown",
		[](const EventListener* listener) { ((Game*)listener)->startGame(); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnPauseButtonDown", 
		[](const EventListener* listener) { ((Game*)listener)->pauseGame(); } 
	);

	m_eventController->addListenerToEvent(
		this,
		"OnGameEnd",
		[](const EventListener* listener) { ((Game*)listener)->pauseGame(); ((Game*)listener)->updateStatistic(); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRestartButtonDown",
		[](const EventListener* listener) { ((Game*)listener)->restartGame();}
	);

	m_eventController->addListenerToEvent(
		this,
		"OnExitButtonDown",
		[](const EventListener* listener) { ((Game*)listener)->exitGame(); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnLeftArrowSpDown",
		[](const EventListener* listener) { ((Game*)listener)->m_heroSkin = (HeroSkin)((((Game*)listener)->m_heroSkin + HeroSkin_Count - 1) % HeroSkin_Count); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRightArrowSpDown",
		[](const EventListener* listener) { ((Game*)listener)->m_heroSkin = (HeroSkin)((((Game*)listener)->m_heroSkin + 1) % HeroSkin_Count); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnLeftArrowP1Down",
		[](const EventListener* listener) { ((Game*)listener)->m_player1Skin = (HeroSkin)((((Game*)listener)->m_player1Skin + HeroSkin_Count - 1) % HeroSkin_Count); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRightArrowP1Down",
		[](const EventListener* listener) { ((Game*)listener)->m_player1Skin = (HeroSkin)((((Game*)listener)->m_player1Skin + 1) % HeroSkin_Count); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnLeftArrowP2Down",
		[](const EventListener* listener) { ((Game*)listener)->m_player2Skin = (HeroSkin)((((Game*)listener)->m_player2Skin + HeroSkin_Count - 1) % HeroSkin_Count); }
	);

	m_eventController->addListenerToEvent(
		this,
		"OnRightArrowP2Down",
		[](const EventListener* listener) { ((Game*)listener)->m_player2Skin = (HeroSkin)((((Game*)listener)->m_player2Skin + 1) % HeroSkin_Count); }
	);

}

void Game::initialize(GameMode mode)
{
	// Clear objects list
	shutdown();

	m_needToInit = false;

	m_currentGameMode = mode;
	m_isGameSessionEnded = false;

	updateStatistic();

	m_score = 0;
	distance = 0;
	lastYpos = 140;

	m_mainCamera->setPosition(Vec2());
	m_requiredCameraYPosition = 0;

	m_physicsController->initialize(this);

	Vec2 ver[4] = { Vec2(-8, -6),Vec2(8, -6), Vec2(8, 9), Vec2(-8, 9) };

	// Create Hero
	if (m_currentGameMode == GameMode_OnePlayer)
	{
		m_player1 = (Hero*)createGameObject(GameObjectType_Hero, 0, 140, 0, getSpriteNameBySkin(m_heroSkin));
		m_player1->setKeys("P1Left", "P1Right", "P1Jump");
		m_player1->setVelocity(Vec2(0, 0));
		m_player1->setUseGravity(true);

		m_player1->setName("Player 1");

		m_player1->setColliderVertex(4, ver);

		m_player2 = 0;
	}
	
	if (m_currentGameMode == GameMode_TwoPlayers)
	{
		m_player1 = (Hero*)createGameObject(GameObjectType_Hero, 100, 140, 0, getSpriteNameBySkin(m_player1Skin));
		m_player1->setKeys("P1Left", "P1Right", "P1Jump");
		m_player1->setVelocity(Vec2(0, 0));
		m_player1->setUseGravity(true);
		m_player1->setColliderVertex(4, ver);
	
		m_player2 = (Hero*)createGameObject(GameObjectType_Hero, -100, 140, 0, getSpriteNameBySkin(m_player2Skin));
		m_player2->setKeys("P2Left", "P2Right", "P2Jump");
		m_player2->setVelocity(Vec2(0, 0));
		m_player2->setUseGravity(true);
		m_player2->setColliderVertex(4, ver);

		m_player1->setName("Player 1");
		m_player2->setName("Player 2");
	}

	m_platformPool->setup(this, GameObjectType_Platform, 10);
	m_cloudPool->setup(this, GameObjectType_Cloud, 5);

	m_controller->initialize(this, m_platformPool, m_cloudPool, m_mainCamera);

	Interface* gameInterface = Interface::instance();
	((UIText*)gameInterface->getInterfaceWindow(MenuType_InGameMenu)->getObject("FPS"))->setChangedValue(&m_fps);
	((UIText*)gameInterface->getInterfaceWindow(MenuType_InGameMenu)->getObject("Score"))->setChangedValue(&m_score);
	((UIText*)gameInterface->getInterfaceWindow(MenuType_PauseMenu)->getObject("Score"))->setChangedValue(&m_score);
	((UIText*)gameInterface->getInterfaceWindow(MenuType_ScoreMenu)->getObject("Score"))->setChangedValue(&m_score);
	((UIText*)gameInterface->getInterfaceWindow(MenuType_StatisticWindow)->getObject("HihgScore"))->setChangedValue(&m_highScore);
	((UIText*)gameInterface->getInterfaceWindow(MenuType_StatisticWindow)->getObject("TimeInGame"))->setChangedValue(&m_minutesInGame);

}

void Game::updateStatistic() {
	if (m_score > m_highScore)
		m_highScore = m_score;

	m_dataManager->getStatistic()->put("MaxScore", m_highScore);
	m_dataManager->getStatistic()->put("TimeInGame", m_timeInGame);
	m_dataManager->writeData(SettingsFile_Statistic);
}

bool Game::frame() {
	// Calculate deltaTime
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - m_lastClock;
	float deltaTime = (float)deltaClock / CLOCKS_PER_SEC;
	m_lastClock = clockNow;

	// Calculate FPS
	t += deltaTime;
	frames++;

	if (t >= 0.5){
		m_fps = (int)std::floor( (frames / t) + 0.5);
		t -= 0.5;
		frames = 0;
	}

	// Update time in game
	m_timeInGame += deltaTime;
	m_minutesInGame = m_timeInGame / 60;

	if (m_needToInit)
		initialize(m_currentGameMode);

	if (m_isGameSessionActive){
		float a;
		m_physicsController->update(deltaTime, a);
		render(a);
		update(deltaTime);
	}

	return !m_isGameEnded;
}

void Game::shutdown()
{
	// Clear objects
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objects[i] != 0)
		{
			delete m_objects[i];
			m_objects[i] = 0;
		}
	}
}

void Game::render(float alpha)
{
	// Sort GameObjects, if some objects change depth;
	if (m_needToSortGameObjects)
		sortGameObjectByDepth();

	// Draw frame
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objects[i])
			m_objects[i]->render(alpha);
	}
}

void Game::update(float dt)
{
	// Update objects
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objects[i] != 0)
			m_objects[i]->update(dt);
	}
	m_controller->update(dt);

	// Move camera 
	if(m_mainCamera->getPosition().y > m_requiredCameraYPosition)
	{
		float vel = 0;
		if (m_currentGameMode == GameMode_OnePlayer) {
			vel = (kCameraSpeed + m_player1->getVelocity().y) / 2;
		}
		else if (m_currentGameMode == GameMode_TwoPlayers) { 
			vel = (kCameraSpeed + m_player1->getVelocity().y + m_player2->getVelocity().y) / 3;
		}
		m_mainCamera->setPosition( m_mainCamera->getRealPosition() + Vec2(0, vel * dt ));
	}

	// Calculate traveled distance
	if (m_player1->getPosition().y < lastYpos)
	{
		distance += abs(m_player1->getPosition().y - lastYpos);
		lastYpos = m_player1->getPosition().y;
	}

	if (m_player2 && m_player2->getPosition().y < lastYpos)
	{
		distance += abs(m_player2->getPosition().y - lastYpos);
		lastYpos = m_player2->getPosition().y;
	}

	m_score = (int)distance / 16;

	// Restart game
	if (m_player1->getPosition().y > m_mainCamera->getCenterPosition().y + kScreenHeight / 2 + 8)
	{
		if (m_currentGameMode == GameMode_TwoPlayers)
		{
			m_eventController->startEvent("OnSecondPlayerWin");
		}
		m_eventController->startEvent("OnGameEnd");
		m_isGameSessionEnded = true;
	}

	if (m_player2 && m_player2->getPosition().y > m_mainCamera->getCenterPosition().y + kScreenHeight / 2 + 8)
	{
		m_eventController->startEvent("OnFirstPlayerWin");
		m_eventController->startEvent("OnGameEnd");
		m_isGameSessionEnded = true;
	}
}

int getObjectDepth(GameObject* object) {
	if (!object) return std::numeric_limits<int>::max();
	return object->getDepth();
}

void sortGameObjects(GameObject** objects, int count)
{
	int i = 0, j = count;
	GameObject* temp;
	float p;
	p = objects[count >> 1]->getDepth();

	do
	{
		while (p < objects[i]->getDepth()) i++;
		while (p > objects[j]->getDepth()) j--;

		if (i <= j)
		{
			temp = objects[i]; objects[i] = objects[j]; objects[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);

	if (j > 0) sortGameObjects(objects, j);
	if (count > i) sortGameObjects(objects + i, count - i);
}

void Game::sortGameObjectByDepth()
{
	int t = 0;
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objects[i])
		{
			if (i != t)
			{
				m_objects[t] = m_objects[i];
				m_objects[i] = 0;
			}
			t++;
		}
	}

	sortGameObjects(m_objects, t - 1);

	m_needToSortGameObjects = false;
}

GameObject* Game::createGameObject(GameObjectType type, float x, float y, float z, std::string spriteName) {

	for (int i = 0; i < kMaxObjectsCount; i++) {
		// Found free space
		if (m_objects[i] == 0) {
			GameObject* object = 0;

			switch (type)
			{
			case GameObjectType_Hero:		object = new Hero();		break;
			case GameObjectType_Platform:	object = new Platform();	break;
			case GameObjectType_Cloud:		object = new Cloud();		break;
			}

			if (object == 0)
				return 0;

			if (spriteName == "") spriteName = getSpriteNameByType(type);

			int spriteSize = m_dataManager->getSpriteParameters()->get<int>(spriteName + ".size", 1);
			SpriteContainer* container = m_dataManager->getSpriteContainer(spriteName);

			object->setCamera(m_mainCamera);
			object->setPosition(Vec2(x, y));
			object->setScale(spriteSize);
			object->setDepth(z);

			object->setTextureRect(container->rect);
			object->setTexture(*container->texturePtr);

			Vec2 tmpSize = Vec2((float)container->rect.width * spriteSize, (float)container->rect.height * spriteSize);
			object->setSize(tmpSize);

			object->calculateMass();
			object->calculateVertexAutomatically();

			m_objects[i] = object;

			return object;
		}
	}

	return 0;
}

std::string getSpriteNameByType(GameObjectType type)
{
	switch (type)
	{
		case GameObjectType_Hero:		return "ChestSprite";
		case GameObjectType_Platform:	return "PlatformSprite";
		case GameObjectType_Cloud:		return "CloudSprite";
	}

	return "None";
}