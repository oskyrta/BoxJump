////////////////////////////////////////////////
// Include
#include "game.h"
#include "levelSettings.h"

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

#include "utils.h"
#include "collision.h"

#include "pool.h"
#include "controller.h"
#include "eventController.h"
#include "settingsManager.h"
#include "physicsController.h"
#include "eventListener.h"

#include "guiText.h"

#include "hero.h"
#include "platform.h"

////////////////////////////////////////////////
// Global
sf::Texture* g_atlas00 = 0;
sf::Font* g_font = 0;
Vec2 zeroVec = Vec2();

extern SettingsManager settingsManager;
extern GUIText* winnerCongratulation;

////////////////////////////////////////////////
// Function declaration
void GetInterval(ObjectPolygon* a, Vec2 axis, float& min0, float& max0);
sf::IntRect getObjectRectByType(GameObjectType type);
float Vec2Dot(Vec2 a, Vec2 axis);

////////////////////////////////////////////////
// Variables
float t = 0;
int frames = 0;
float distance = 0;
float lastYpos = 0;

static double fpsLock = 0;

////////////////////////////////////////////////
// Class Game

Game::Game()
{
	m_isGameActive = true;
	
	m_gameState = false;

	m_lastClock = 0;
	m_fps = 0;

	m_score = 0;
	m_maxScore = 0;
	m_timeInGame = 0;

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		m_objects[i] = 0;
	}
	m_player1 = 0;
	m_player2 = 0;
	m_platformPool = 0;
	m_controller = 0;
	m_physicsController = 0;

	m_mainCamera = 0;
	m_requiredCameraYPosition = 0;
	m_eventController = 0;

	g_font = 0;
}

Game::~Game()
{
	if (g_atlas00)
		delete g_atlas00;

	if (g_font)
		g_font = 0;

	updateStatistic();
}

void Game::setupSystem()
{
	// Set seed to random generator
	srand(time(0));

	// Set start clock
	m_lastClock = clock();

	// Initialize pool
	m_platformPool = new Pool();

	// Initialize level controller
	m_controller = new Controller();

	m_physicsController = new PhysicsController();

	// Load texture
	g_atlas00 = new sf::Texture();
	g_atlas00->loadFromFile("atlas00.png");

	// Load font
	g_font = new sf::Font();
	g_font->loadFromFile("stan0753.ttf");

	fpsLock = settingsManager.p_windowSettings->get<int>("FPSLock");
	if (fpsLock != 0) fpsLock = 1.0 / fpsLock;

	m_timeInGame = settingsManager.p_statistic->get<double>("TimeInGame", 0);
	m_minutesInGame = m_timeInGame / 60;
	m_maxScore = settingsManager.p_statistic->get<int>("MaxScore", 0);

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_Start1pGameButtonDown);
	m_functions[GameEvent_Start1pGameButtonDown] = [](const EventListener* listener) { ((Game*)listener)->startNew1PGame(); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_Start2pGameButtonDown);
	m_functions[GameEvent_Start2pGameButtonDown] = [](const EventListener* listener) { ((Game*)listener)->startNew2PGame(); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_StartButtonDown);
	m_functions[GameEvent_StartButtonDown] = [](const EventListener* listener) { ((Game*)listener)->startGame(); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_PauseButtonDown);
	m_functions[GameEvent_PauseButtonDown] = [](const EventListener* listener) { ((Game*)listener)->pauseGame(); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_GameEnd);
	m_functions[GameEvent_GameEnd] = [](const EventListener* listener) { ((Game*)listener)->pauseGame(); ((Game*)listener)->updateStatistic(); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_RestartButtonDown);
	m_functions[GameEvent_RestartButtonDown] = [](const EventListener* listener) { ((Game*)listener)->restartGame(); };

	m_eventController->addListenerToEvent((EventListener*)this, GameEvent_ExitButtonDown);
	m_functions[GameEvent_ExitButtonDown] = [](const EventListener* listener) { ((Game*)listener)->exitGame(); };
}

void Game::initialize(GameMode mode)
{
	// Clear objects list
	shutdown();

	needToInit = false;

	m_currentGameMode = mode;
	m_gameEnded = false;

	updateStatistic();

	m_score = 0;
	distance = 0;
	lastYpos = -100;

	m_mainCamera->setPosition(Vec2());
	m_requiredCameraYPosition = 0;

	m_physicsController->initialize(this);

	// Create Hero
	if (m_currentGameMode == GameMode_OnePlayer)
	{
		m_player1 = (Hero*)createGameObject(GameObjectType_Hero, 0, -100, kBoxImage);
		m_player1->setKeys(VK_LEFT, VK_RIGHT, VK_UP);
		m_player1->setVelocity(Vec2(0, 0));
		m_player1->setUseGravity(true);

		m_player1->setName("Player 1");

		m_player2 = 0;
	}
	
	if (m_currentGameMode == GameMode_TwoPlayers)
	{
		m_player1 = (Hero*)createGameObject(GameObjectType_Hero, 100, 159, kBoxImage);
		m_player1->setKeys(VK_LEFT, VK_RIGHT, VK_UP);
		m_player1->setVelocity(Vec2(0, 0));
		m_player1->setUseGravity(true);
	
		m_player2 = (Hero*)createGameObject(GameObjectType_Hero, -100, 159, kBoxImage);
		m_player2->setKeys('A', 'D', 'W');
		m_player2->setVelocity(Vec2(0, 0));
		m_player2->setUseGravity(true);

		m_player1->setName("Player 1");
		m_player2->setName("Player 2");
	}

	m_platformPool->setup(this, GameObjectType_Platform, 10);

	m_controller->initialize(this, m_platformPool, m_mainCamera);
}

void Game::updateStatistic()
{
	if (m_score > m_maxScore)
		m_maxScore = m_score;

	settingsManager.p_statistic->put("MaxScore", m_maxScore);
	settingsManager.p_statistic->put("TimeInGame", m_timeInGame);
	settingsManager.writeData(SettingsFile_Statistic);
}

bool Game::frame()
{
	// Exit from main cycle if window is closed
	if(!m_mainCamera->cameraFrame())
	{
		return false;
	}

	// Calculate deltaTime
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - m_lastClock;
	float deltaTime = (float)deltaClock / CLOCKS_PER_SEC;
	m_lastClock = clockNow;

	// Calculate FPS
	t += deltaTime;
	frames++;

	if (t >= 0.5)
	{
		m_fps = std::floor( (frames / t) + 0.5);
		t -= 0.5;
		frames = 0;
	}

	// Update time in game
	m_timeInGame += deltaTime;
	m_minutesInGame = m_timeInGame / 60;

	if (needToInit)
		initialize(m_currentGameMode);

	if (m_gameState)
	{
		float a;
		m_physicsController->update(deltaTime, a);
		render(a);
		update(deltaTime);
	}

	if (fpsLock > 0)
	{
		clockNow = clock();
		deltaClock = clockNow - m_lastClock;
		double deltaTime = (double)deltaClock / CLOCKS_PER_SEC;

		if (deltaTime < fpsLock)
		{
			std::chrono::nanoseconds duration(static_cast<long>((fpsLock - deltaTime) * 10000000));
			std::this_thread::sleep_for(duration);
		}
	}

	return m_isGameActive;
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
	if (m_mainCamera->getPosition().y < m_requiredCameraYPosition)
	{
		m_mainCamera->setPosition(Vec2(0, m_requiredCameraYPosition));
	}
	else if(m_mainCamera->getPosition().y > m_requiredCameraYPosition)
	{
		m_mainCamera->setPosition(Vec2(m_mainCamera->getPosition() + Vec2(0, kCameraSpeed * dt) ));
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
	if (m_player1->getPosition().y > m_mainCamera->getPosition().y + kPixlelsInRow / 2 + 16)
	{
		if (m_currentGameMode == GameMode_TwoPlayers)
		{
			m_eventController->startEvent(GameEvent_SecondPlayerWin);
		}
		m_eventController->startEvent(GameEvent_GameEnd);
		m_gameEnded = true;
	}

	if (m_player2 && m_player2->getPosition().y > m_mainCamera->getPosition().y + kPixlelsInRow / 2 + 16)
	{
		m_eventController->startEvent(GameEvent_FirstPlayerWin);
		m_eventController->startEvent(GameEvent_GameEnd);
		m_gameEnded = true;
	}
}

GameObject* Game::createGameObject(GameObjectType type, float x, float y, sf::IntRect rect)
{
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		// Found free space
		if (m_objects[i] == 0)
		{
			GameObject* object = 0;

			switch (type)
			{
				case GameObjectType_Hero:		object = new Hero(); break;
				case GameObjectType_Platform:	object = new Platform(); break;
			}

			if (object == 0)
				return 0;

			object->setGame(this);
			object->setCamera(m_mainCamera);
			object->setPosition(Vec2(x, y));
			object->setScale((float)m_mainCamera->getPixelSize());

			if (rect.height == 0 && rect.width == 0) rect = getObjectRectByType(type);

			object->setTextureRect(rect);

			Vec2 tmpSize = Vec2((float) rect.width * m_mainCamera->getPixelSize(), (float) rect.height * m_mainCamera->getPixelSize());
			object->setSize(tmpSize);

			object->calculateMass();

			object->calculateVertexAutomatically();

			m_objects[i] = object;

			return m_objects[i];
		}
	}

	return 0;
}

sf::IntRect getObjectRectByType(GameObjectType type)
{
	switch (type)
	{
	case GameObjectType_Hero:		return kBoxImage;
	case GameObjectType_Platform:	return kPlatformImage;
	}

	return sf::IntRect(0, 0, 0, 0);
}