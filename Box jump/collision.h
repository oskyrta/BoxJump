#pragma once
////////////////////////////////////////////////
// Include
#include "vec2.h"

////////////////////////////////////////////////
// Forvard declaration
class GameObject;
enum GameObjectType;

////////////////////////////////////////////////
// Enum
enum CollisionState
{
	CollisionState_None,
	CollisionState_Start,
	CollisionState_Stay,
	CollisionState_Exit
};

////////////////////////////////////////////////
// Struct
struct ObjectsPair
{
	GameObject* object1;
	GameObject* object2;

	bool operator== (const ObjectsPair& a);
};

////////////////////////////////////////////////
// Class Collision
class Collision
{
public:
	Collision();

	void setObjects(GameObject* object1 = 0, GameObject* object2 = 0) { m_objects.object1 = object1, m_objects.object2 = object2; };
	ObjectsPair getObjects() { return m_objects; };

	void setNormal(Vec2 normal) { m_normal = normal.normalize(); };
	Vec2 getNormal() { return m_normal; };

	void setDepth(float depth) { m_depth = depth; };
	float getDepth() { return m_depth; }

	void setEnterDiretction(Vec2 enterDirection) { m_enterDirection = enterDirection; };
	Vec2 getEnterDirection() { return m_enterDirection; };

	void setState(CollisionState state) { m_state = state; };
	CollisionState getState() { return m_state; };

	bool collisionWith(GameObjectType type);

	bool m_needToResolve;

	void clear();

private:
	ObjectsPair m_objects;
	Vec2 m_enterDirection;

	CollisionState m_state;

	Vec2 m_normal;
	float m_depth;

};