#pragma once
/////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>
#include <ctime>

#include "camera.h"
#include "gameObjectType.h"
#include "levelSettings.h"
#include "collision.h"


/////////////////////////////////////////////////
// Forward declaration
class GameObject;
class Hero;
class Pool;
class Controller;
class EventController;
class PhysicsController;

/////////////////////////////////////////////////
// Class Game
class Game
{
public:
	Game();
	~Game();

	void setupSystem();
	void initialize();
	bool frame();
	void shutdown();

	//bool intersects(GameObject* object, Collision* coll[]);

	void setEventController(EventController* ec) { m_eventController = ec; };

	bool getGameState() { return m_gameState; };
	bool getGameEnded() { return m_gameEnded; };
	void startGame() { m_gameState = true; m_gameEnded = false; };
	void pauseGame() { m_gameState = false; };

	void setCamera(Camera* camera) { m_mainCamera = camera; };
	Camera* getCamera() { return m_mainCamera; };

	const int* getPointerToFPS() { return &m_fps; };
	const int* getPointerToScore() { return &m_score; };
	const int* getPointerToMaxScore() { return &m_maxScore; };
	const double* getPointerToTimeIngame() { return &m_timeInGame; };
	GameObject** getObjects() { return m_objects; };

	void setRequiredCameraPos(float yPos) { if(yPos < m_requiredCameraYPosition) m_requiredCameraYPosition = yPos; };

	GameObject* createGameObject(GameObjectType type, float x, float y, sf::IntRect rect = sf::IntRect(0, 0, 0, 0));
private:
	void update(float dt);
	void render(float alpha);

	void updateStatistic();

private:
	bool m_isGameActive;
	bool m_gameEnded;
	bool m_gameState;

	EventController* m_eventController;
	PhysicsController* m_physicsController;

	int m_fps;
	clock_t m_lastClock;

	int m_score;
	int m_maxScore;
	double m_timeInGame;

	Camera* m_mainCamera;
	float m_requiredCameraYPosition;

	sf::RenderWindow* m_renderWindow;

	Pool* m_platformPool;
	Controller* m_controller;

	GameObject* m_objects[kMaxObjectsCount];
	Hero* m_hero;
};