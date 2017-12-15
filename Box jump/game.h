#pragma once
/////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>
#include <ctime>

#include "camera.h"
#include "gameObjectType.h"
#include "levelSettings.h"
#include "collision.h"
#include "eventListener.h"


/////////////////////////////////////////////////
// Forward declaration
class GameObject;
class Hero;
class Pool;
class Controller;
class EventController;
class PhysicsController;
enum HeroSkin;

/////////////////////////////////////////////////
// Enum
enum GameMode
{
	GameMode_OnePlayer,
	GameMode_TwoPlayers
};

/////////////////////////////////////////////////
// Class Game
class Game: public EventListener
{
public:
	Game();
	~Game();

	void setupSystem();
	void initialize(GameMode mode);
	bool frame();
	void shutdown();

	void setEventController(EventController* ec) { m_eventController = ec; };

	bool getGameState() { return m_gameState; };
	bool getGameEnded() { return m_gameEnded; };

	void startGame() { m_gameState = true; m_gameEnded = false; };
	void restartGame() { m_needToInit = true; startGame(); }
	void pauseGame() { m_gameState = false; };
	void startNew1PGame() { m_needToInit = true; m_currentGameMode = GameMode_OnePlayer; startGame(); }
	void startNew2PGame() { m_needToInit = true; m_currentGameMode = GameMode_TwoPlayers; startGame(); }
	void exitGame() { m_isGameActive = false; }

	void setCamera(Camera* camera) { m_mainCamera = camera; };
	Camera* getCamera() { return m_mainCamera; };

	void sortGameObjectsBeforeRender() { m_needToSortGameObjects = true; };

	GameMode getGameMode() { return m_currentGameMode; };
	const int* getPointerToFPS() { return &m_fps; };
	const int* getPointerToScore() { return &m_score; };
	const int* getPointerToMaxScore() { return &m_maxScore; };
	const int* getPointerToTimeInGame() { return &m_minutesInGame; };
	GameObject** getObjects() { return m_objects; };

	void setRequiredCameraPos(float yPos) { if(yPos < m_requiredCameraYPosition) m_requiredCameraYPosition = yPos; };

	GameObject* createGameObject(GameObjectType type, float x, float y, float z = 0, std::string spriteName = "");
private:
	void update(float dt);
	void render(float alpha);

	void updateStatistic();
	void startListeningEvents();
	void sortGameObjectByDepth();

private:
	bool m_isGameActive;
	bool m_gameEnded;
	bool m_gameState;
	GameMode m_currentGameMode;
	bool m_needToInit;
	bool m_needToSortGameObjects;

	EventController* m_eventController;
	PhysicsController* m_physicsController;

	int m_fps;
	clock_t m_lastClock;

	int m_score;
	int m_maxScore;
	double m_timeInGame;
	int m_minutesInGame;

	Camera* m_mainCamera;
	float m_requiredCameraYPosition;

	sf::RenderWindow* m_renderWindow;

	Pool* m_platformPool;
	Pool* m_cloudPool;
	Controller* m_controller;

	GameObject* m_objects[kMaxObjectsCount];
	Hero* m_player1;
	Hero* m_player2;

	HeroSkin m_heroSkin;
	HeroSkin m_player1Skin;
	HeroSkin m_player2Skin;
};