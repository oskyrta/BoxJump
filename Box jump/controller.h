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

	void initialize(Game* game, Pool* platformPool, Camera* camera);
	void loadLevel();
	void update(float dt);

	void createNextPlatforms(Vec2 currentPlatformPos);
private:
	Platform* m_platformList[kMaxObjectsCount];
	Game* m_game;
	Pool* m_pool;
	Camera* m_camera;
};