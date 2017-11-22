#pragma once
//////////////////////////////////////////////////
// Include
#include "levelSettings.h"

//////////////////////////////////////////////////
// Forward declaration
class Game;
class GameObject;
class Collision;
class Vec2;
struct ObjectPolygon;

//////////////////////////////////////////////////
// Class PhysicsController
class PhysicsController
{
public:
	PhysicsController();

	void initialize(Game* game);
	void update(float dt, float& alpha);

	void setGame(Game* game) { m_game = game; };

private:
	void updatePhysics();
	void checkCollisions();
	bool ObjectsIntersectedOnNormal(ObjectPolygon* a, ObjectPolygon* b, Vec2 axis, float& depth);
	void useGravity();
	void resolveCollisions();
	void resolveCollision(Collision* coll);
	void sendCollisionsToObjects();

private:
	Game* m_game;

	int m_ups;
	float m_deltatime;
	float m_accumulator;

	GameObject** m_objects;

	Collision* m_collisionList[kMaxObjectsCount];
	Collision* m_lastUpdateCollisionList[kMaxObjectsCount];
};