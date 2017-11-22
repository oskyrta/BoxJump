#pragma once
/////////////////////////////////////////////////
// Include
#include "gameObject.h"

/////////////////////////////////////////////////
// Class Hero

class Hero : public GameObject
{
public:
	Hero();

	virtual void update(float dt);
	virtual void physicsUpdate(float dt);

	virtual void intersect(Collision* collision);
	virtual void collisionEnter(Collision* collision);
	virtual void collisionStay(Collision* collision);
	virtual void collisionExit(Collision* collision);

	void setKeys(int leftKey, int rightKey, int fireKey);

private:
	int m_leftKey;
	int m_rightKey;
	int m_fireKey;

	bool m_isJump;

	float m_heroJumpForse;
	float m_heroMoveForse;

	float m_heroSpeed;
	float m_heroMaxSpeed;
};