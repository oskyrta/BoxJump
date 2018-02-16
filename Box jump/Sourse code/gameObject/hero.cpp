/////////////////////////////////////////////////
// Include
#include "gameObject\hero.h"
#include "tools\utils.h"
#include "dataManager/levelSettings.h"
#include "dataManager\dataManager.h"
#include "gameObject\platform.h"

#include <iostream>
#include <Windows.h>

/////////////////////////////////////////////////
// Global 
static const float maxSpeed = 420;
static const float minSpeed = -450;
static int t = 0;

/////////////////////////////////////////////////
// Class Hero

Hero::Hero()
{
	m_type = GameObjectType_Hero;
	m_heroSkin = HeroSkin_Chest;

	m_leftKey = 0;
	m_rightKey = 0;
	m_jumpKey = 0;

	m_averageSpeed = 0;

	DataManager* dataManager = DataManager::instance();
	m_material.density = dataManager->getObjectsSettgins()->get<float>("Wood.Density");
	m_material.restitution = dataManager->getObjectsSettgins()->get<float>("Wood.Restitution");
	m_material.staticFriction = dataManager->getObjectsSettgins()->get<float>("Wood.StaticFriction");
	m_material.dynamicFriction = dataManager->getObjectsSettgins()->get<float>("Wood.DynamicFriction");

	m_jumpForse = dataManager->getObjectsSettgins()->get<float>("Hero.JumpForse");
	m_moveForse = dataManager->getObjectsSettgins()->get<float>("Hero.MoveForse");
	m_maxHorizontalSpeed = dataManager->getObjectsSettgins()->get<float>("Hero.MaxSpeed");

	m_jump_key_was_pressed = false;
}

void Hero::update(float dt)
{
	// Hero move
	if (IsKeyDown(m_leftKey))
	{
		m_currentSpeed = -m_moveForse;
	}

	if (IsKeyDown(m_rightKey))
	{
		m_currentSpeed = m_moveForse;
	}

	if (IsKeyDown(m_rightKey) == IsKeyDown(m_leftKey))
		m_currentSpeed = 0;


	if (!IsKeyDown(m_jumpKey)) m_jump_key_was_pressed = false;

	GameObject::update(dt);
}

void Hero::spriteUpdate()
{
	sf::IntRect rect = m_sprite->getTextureRect();

	if (m_velocity.y < minSpeed / 4 * 3) rect.left = 16;
	else if (m_velocity.y < 20) rect.left = 0;
	else if (m_velocity.y < maxSpeed / 10) rect.left = 32;
	else if (m_velocity.y < maxSpeed / 5) rect.left = 48;
	else rect.left = 64;

	if ( m_velocity.y - m_averageSpeed < -35 && m_averageSpeed > 0) rect.left = 80;

	m_sprite->setTextureRect(rect);

	m_averageSpeed = (m_averageSpeed * 9/10 + m_velocity.y / 10);
}

void Hero::physicsUpdate(float dt)
{
	// Move camera if hero hero is above the screen
	if ((m_position - m_camera->getCenterPosition()).y < -24) m_game->setRequiredCameraPos(m_position.y - 24);

	if (IsKeyDown(m_jumpKey))
		m_jump_key_was_pressed = true;

	if (m_velocity.y < -maxSpeed * 1.2f)
		m_velocity.y = -maxSpeed * 1.2f;

	// Add forse for horizontal move
	if (fabs(m_velocity.x) < m_maxHorizontalSpeed || m_currentSpeed * m_velocity.x < 0)
		addForse( Vec2(m_currentSpeed, 0) );

	// Add air resistance
	addForse(m_velocity * - 1);

	spriteUpdate();

	GameObject::physicsUpdate(dt);
}

void Hero::intersect(Collision* collision)
{
	if (collision->collisionWith(GameObjectType_Platform, GameObjectType_Hero)
		&& 
		(collision->getNormal().y >= 0 || ( collision->getState() == CollisionState_Stay && collision->getEnterDirection().y >= 0) )
	)
	{
		collision->m_needToResolve = false;
		return;
	}

	
	if(	(collision->collisionWith(GameObjectType_Platform, GameObjectType_Hero) || collision->collisionWith(GameObjectType_Hero, GameObjectType_Hero))
		&& 
		collision->getNormal().y < 0 
	)
	{
		GameObject* tmp = 0;
		if (collision->getObjects().object1 == ((GameObject*)this)) 
			tmp = collision->getObjects().object2;
		else 
			tmp = collision->getObjects().object1;

		// Move camera
		m_game->setRequiredCameraPos( tmp->getPosition().y - 180);

		// Check conditions for jump
		if(	((collision->getState() == CollisionState_Stay && !m_jump_key_was_pressed) || collision->getState() == CollisionState_Start)
			&& 
			!m_isJump && IsKeyDown(m_jumpKey)
		)
		{
			m_isJump = true;
			m_forse.y += -m_jumpForse * 10000;

			if (tmp->getType() == GameObjectType_Platform)
			{
				if (((Platform*)tmp)->getPlatformState() & PlatformState_Broken)
					((Platform*)tmp)->setActive(false);
			}
		}
	}
	
}


void Hero::collisionEnter(Collision* collision)
{
	m_isJump = false;
	intersect(collision);
	GameObject::collisionEnter(collision);
}

void Hero::collisionStay(Collision* collision)
{
	intersect(collision);
	GameObject::collisionStay(collision);
}

void Hero::collisionExit(Collision* collision)
{
	GameObject::collisionExit(collision);
}

void Hero::setKeys(int leftKey, int rightKey, int fireKey)
{
	m_leftKey = leftKey;
	m_rightKey = rightKey;
	m_jumpKey = fireKey;
}