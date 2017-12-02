#pragma once
/////////////////////////////////////////////////
// Include
#include "gameObject.h"
#include "eventListener.h"

/////////////////////////////////////////////////
// Enum
enum HeroSkin
{
	HeroSkin_Chest,
	HeroSkin_Pallet
};

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

	void setKeys(int leftKey, int rightKey, int jumpKey);

private:
	void spriteUpdate();

private:
	int m_leftKey;
	int m_rightKey;
	int m_jumpKey;

	bool m_isJump; 
	bool m_jump_key_pressed_in_last_frame;

	HeroSkin m_heroSkin;

	float m_heroJumpForse;
	float m_heroMoveForse;

	float m_heroAverageSpeed;
	float m_heroSpeed;
	float m_heroMaxSpeed;
};