/////////////////////////////////////////////////
// Include
#include "hero.h"
#include "utils.h"
#include "levelSettings.h"
#include "settingsManager.h"
#include "platform.h"

#include <iostream>
#include <Windows.h>

/////////////////////////////////////////////////
// Global 
Vec2 enterDirection = Vec2();

extern SettingsManager settingsManager;

static bool jump_key_pressed_in_last_frame = false;

/////////////////////////////////////////////////
// Class Hero

Hero::Hero()
{
	m_type = GameObjectType_Hero;

	m_leftKey = 0;
	m_rightKey = 0;
	m_jumpKey = 0;

	m_material.density = settingsManager.p_objectsSettings->get<float>("Wood.Density");
	m_material.restitution = settingsManager.p_objectsSettings->get<float>("Wood.Restitution");
	m_material.staticFriction = settingsManager.p_objectsSettings->get<float>("Wood.StaticFriction");
	m_material.dynamicFriction = settingsManager.p_objectsSettings->get<float>("Wood.DynamicFriction");

	m_heroJumpForse = settingsManager.p_objectsSettings->get<float>("Hero.JumpForse");
	m_heroMoveForse = settingsManager.p_objectsSettings->get<float>("Hero.MoveForse");
	m_heroMaxSpeed = settingsManager.p_objectsSettings->get<float>("Hero.MaxSpeed");
}

void Hero::update(float dt)
{
	// Hero move
	if (IsKeyDown(m_leftKey))
	{
		m_heroSpeed = -m_heroMoveForse;
	}

	if (IsKeyDown(m_rightKey))
	{
		m_heroSpeed = m_heroMoveForse;
	}

	if (IsKeyDown(m_rightKey) == IsKeyDown(m_leftKey))
		m_heroSpeed = 0;


	if (!IsKeyDown(VK_SPACE)) jump_key_pressed_in_last_frame = false;

	GameObject::update(dt);
}

void Hero::physicsUpdate(float dt)
{
	if (IsKeyDown(VK_SPACE))
		jump_key_pressed_in_last_frame = true;

	if (fabs(m_velocity.x) < m_heroMaxSpeed || m_heroSpeed * m_velocity.x < 0) 
		addForse( Vec2(m_heroSpeed, 0) );

	// Add air resistance
	addForse(m_velocity * - 1);

	GameObject::physicsUpdate(dt);
}

void Hero::intersect(Collision* collision)
{
	if (collision->collisionWith(GameObjectType_Platform) && (collision->getNormal().y >= 0 || (collision->getState() == CollisionState_Stay && collision->getEnterDirection().y >= 0) && m_velocity.y <=0))
	{
		collision->m_needToResolve = false;
		return;
	}
	
	if(( (collision->getState() == CollisionState_Stay && !jump_key_pressed_in_last_frame) || collision->getState() == CollisionState_Start )
		&&
		(collision->collisionWith(GameObjectType_Platform) || collision->collisionWith(GameObjectType_Hero))
		&& 
		collision->getNormal().y < 0 
		)
	{
		m_game->setRequiredCameraPos( collision->getObjects().object2->getPosition().y - 175 );
		if (!m_isJump && IsKeyDown(m_jumpKey))
		{
			m_isJump = true;
			m_forse.y += -m_heroJumpForse * 10000;
			Platform* tmpPlatform = (Platform*)collision->getObjects().object2;
			if (tmpPlatform->getPlatformState() & PlatformState_Broken)
				collision->getObjects().object2->setActive(false);
		}
	}
	
}


void Hero::collisionEnter(Collision* collision)
{
	m_isJump = false;
	intersect(collision);
	//std::cout << "Enter ";
	GameObject::collisionEnter(collision);
}

void Hero::collisionStay(Collision* collision)
{
	intersect(collision);
	//std::cout << "Stay ";
	GameObject::collisionStay(collision);
}

void Hero::collisionExit(Collision* collision)
{
	//std::cout << "Exit ";
	GameObject::collisionExit(collision);
}

void Hero::setKeys(int leftKey, int rightKey, int fireKey)
{
	m_leftKey = leftKey;
	m_rightKey = rightKey;
	m_jumpKey = fireKey;
}