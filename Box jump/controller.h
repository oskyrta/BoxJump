#pragma once
//////////////////////////////////////////////////
// Include
#include "gameObject.h"

///////////////////////////////////////////////////
// Forvard declaration
class Hero;
class Camera;
class Pool;
class Platform;

//////////////////////////////////////////////////
// Class Controller
class Controller
{
public:
	Controller();

	void initialize(Pool* platformPool, Camera* camera);
	void loadLevel();
	void update(float dt);

	void createNextPlatforms(Vec2 currentPlatformPos);
private:
	Platform* m_platformList[kMaxObjectsCount];
	Hero* m_hero;
	Pool* m_pool;
	Camera* m_camera;
};