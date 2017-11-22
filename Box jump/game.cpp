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

#include "hero.h"
#include "platform.h"

////////////////////////////////////////////////
// Global
sf::Texture* g_atlas00 = 0;
sf::Font* g_font = 0;
Vec2 zeroVec = Vec2();

extern SettingsManager settingsManager;

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
	m_hero = 0;
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
	// Set seed
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
	m_maxScore = settingsManager.p_statistic->get<int>("MaxScore", 0);
}

void Game::initialize()
{
	// Clear objects list
	shutdown();

	m_score = 0;
	distance = 0;
	lastYpos = 0;

	m_mainCamera->setPosition(Vec2());
	m_requiredCameraYPosition = 0;

	m_physicsController->initialize(this);

	// Create Hero
	m_hero = (Hero*)createGameObject(GameObjectType_Hero, 0, -100, kBoxImage);
	m_hero->setKeys(VK_LEFT, VK_RIGHT, VK_UP);
	m_hero->setVelocity(Vec2(0, 0));
	m_hero->setUseGravity(true);

	m_platformPool->setup(this, GameObjectType_Platform, 10);

	m_controller->initialize(m_platformPool, m_mainCamera);
}

void Game::updateStatistic()
{
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

	if (t >= 1.0)
	{
		t -= 1.0;
		m_fps = frames;
		frames = 0;
	}

	m_timeInGame += deltaTime;

	// Check events
	if (m_eventController)
	{
		if (!m_gameState && m_eventController->getEventState(GameEvent_StartButtonDown))
		{
			startGame();
			std::cout << "Game started\n";
		}

		if (m_gameState && m_eventController->getEventState(GameEvent_PauseButtonDown))
		{
			pauseGame();
			std::cout << "Game paused\n";
		}

		if (m_eventController->getEventState(GameEvent_MainMenuButtonDown))
		{
			m_gameEnded = false;
			initialize();
		}

		if (m_eventController->getEventState(GameEvent_GameEnd))
		{
			pauseGame();
			if (m_score > m_maxScore)
				m_maxScore = m_score;
			updateStatistic();
		}

		if (m_eventController->getEventState(GameEvent_RestartButtonDown))
		{
			initialize();
			startGame();
			m_gameEnded = false;
		}

		if (m_eventController->getEventState(GameEvent_ExitButtonDown))
		{
			return false;
		}
	}

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
			std::chrono::nanoseconds duration(static_cast<long>((fpsLock - deltaTime) * 1000000));
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
		{
			m_objects[i]->render(alpha);
		}
	}
}

void Game::update(float dt)
{
	// Update objects
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objects[i] != 0)
		{
			m_objects[i]->update(dt);
		}
	}
	m_controller->update(dt);

	if (m_mainCamera->getPosition().y < m_requiredCameraYPosition)
	{
		m_mainCamera->setPosition(Vec2(0, m_requiredCameraYPosition));
	}
	else if(m_mainCamera->getPosition().y > m_requiredCameraYPosition)
	{
		m_mainCamera->setPosition(Vec2(m_mainCamera->getPosition() + Vec2(0, kCameraSpeed * dt) ));
	}

	// Calculate traveled distance
	if (m_hero->getPosition().y < lastYpos)
	{
		distance += abs(m_hero->getPosition().y - lastYpos);
		lastYpos = m_hero->getPosition().y;
	}

	m_score = (int)distance / 16;

	// Restart game
	if (m_hero->getPosition().y > m_mainCamera->getPosition().y + kPixlelsInRow / 2 + 16)
	{
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