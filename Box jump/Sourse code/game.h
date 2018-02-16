#pragma once
/////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>
#include <ctime>
#include <vector>

#include "render\camera.h"
#include "gameObject/gameObjectType.h"
#include "dataManager/levelSettings.h"
#include "physics/collision.h"
#include "eventSystem/eventListener.h"


/////////////////////////////////////////////////
// Forward declaration
class GameObject;
class Hero;
class Pool;
class Controller;
class EventController;
class PhysicsController;
class DataManager;
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

	// Single initialization
	static Game* instance()
	{
		static Game *instance_ = new Game();
		return instance_;
	};

	void setupSystem();
	void initialize(GameMode mode);
	bool frame();
	void shutdown();

	void setEventController(EventController* ec) { m_eventController = ec; };

	bool getGameState() { return m_isGameSessionActive; };
	bool getGameEnded() { return m_isGameSessionEnded; };

	void startGame() { m_isGameSessionActive = true; m_isGameSessionEnded = false; };
	void restartGame() { m_needToInit = true; startGame(); }
	void pauseGame() { m_isGameSessionActive = false; };
	void startNew1PGame() { m_needToInit = true; m_currentGameMode = GameMode_OnePlayer; startGame(); }
	void startNew2PGame() { m_needToInit = true; m_currentGameMode = GameMode_TwoPlayers; startGame(); }
	void exitGame() { m_isGameEnded = true; }

	void setCamera(Camera* camera) { m_mainCamera = camera; };
	Camera* getCamera() { return m_mainCamera; };

	void sortGameObjectsBeforeRender() { m_needToSortGameObjects = true; };

	GameMode getGameMode() { return m_currentGameMode; };
	const int* getPointerToFPS() { return &m_fps; };
	const int* getPointerToScore() { return &m_score; };
	const int* getPointerToMaxScore() { return &m_highScore; };
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
	bool m_isGameEnded;
	bool m_isGameSessionEnded;
	bool m_isGameSessionActive;
	GameMode m_currentGameMode;
	bool m_needToInit;
	bool m_needToSortGameObjects;

	EventController* m_eventController;
	PhysicsController* m_physicsController;
	DataManager* m_dataManager;

	int m_fps;
	clock_t m_lastClock;

	int m_score;
	int m_highScore;
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