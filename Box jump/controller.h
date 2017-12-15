#pragma once
//////////////////////////////////////////////////
// Include
#include "gameObject.h"

///////////////////////////////////////////////////
// Forvard declaration
class Game;
class Camera;
class Pool;
class Platform;

//////////////////////////////////////////////////
// Class Controller
class Controller
{
public:
	Controller();

	void initialize(Game* game, Pool* platformPool, Pool* cloudPool, Camera* camera);
	void loadLevel();
	void update(float dt);

private:
	void createNextPlatforms(Vec2 currentPlatformPos);
	void createNextCloud(Vec2 cloudPos);

private:
	GameObject* m_objectsList[kMaxObjectsCount];
	Game* m_game;
	Pool* m_platformPool;
	Pool* m_cloudPool;
	Camera* m_camera;
};