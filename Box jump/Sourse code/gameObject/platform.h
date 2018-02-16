#pragma once
/////////////////////////////////////////////////
// Include
#include "gameObject\gameObject.h"

/////////////////////////////////////////////////
// Enum
enum Platform_State
{
	PlatformState_Simple	= 1,
	PlatformState_Broken	= 1 << 1,
	PlatformState_VMove		= 1 << 2,
	PlatformState_HMove		= 1 << 3 
};

/////////////////////////////////////////////////
// Class Platform

class Platform : public GameObject
{
public:
	Platform();
	~Platform();

	void initialize();
	void initialize(Vec2 pos);
	virtual void physicsUpdate(float dt);

	virtual void intersect(Collision* coll);
	virtual void collisionEnter(Collision* collision);
	virtual void collisionStay(Collision* collision);
	virtual void collisionExit(Collision* collision);

	int getPlatformState() { return m_state; };
	Vec2 getStartPosition() { return m_startPosition; };

private:
	int m_state;
	float m_time;
	float m_platformSpeed;

	float m_apmlitude;
	float m_f;

	Vec2 m_startPosition;
};