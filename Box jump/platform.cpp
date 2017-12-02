/////////////////////////////////////////////////
// Include
#include "platform.h"
#include "settingsManager.h"
#include "utils.h"

extern SettingsManager settingsManager;
/////////////////////////////////////////////////
// Class Platform

Platform::Platform()
{
	m_type = GameObjectType_Platform;

	m_state = PlatformState_Simple;
	m_time = 0;

	m_platformSpeed = 0;
	m_apmlitude = 0;
	m_f = 0;

	m_material.density = settingsManager.p_objectsSettings->get<float>("Platform.Density");
	m_material.restitution = settingsManager.p_objectsSettings->get<float>("Platform.Restitution");
	m_material.staticFriction = settingsManager.p_objectsSettings->get<float>("Platform.StaticFriction");
	m_material.dynamicFriction = settingsManager.p_objectsSettings->get<float>("Platform.DynamicFriction");
}

Platform::~Platform()
{

}

void Platform::initialize(Vec2 pos)
{
	m_position = pos;
	initialize();
}

void Platform::initialize()
{
	m_time = 0;
	m_state = 1;

	m_startPosition = m_position;

	m_platformSpeed = 10;
	m_apmlitude = 100;
	m_f = GetRandomFloat(0, 3.1417f);

	m_velocity = Vec2();

	int stateCount = 0;

	int t = GetRandomInt(0, 10);
	if (t < 5) stateCount = 0;
	else if (t < 9) stateCount = 1;
	else stateCount = 2;

	for (int i = 0; i < stateCount; i++)
	{
		m_state |= 1 << GetRandomInt(0, 3);
	}

	m_isActive = true;

	if (m_state & PlatformState_Broken)
		m_sprite->setTextureRect(kBrokenPlatformImage);
	else
		m_sprite->setTextureRect(kPlatformImage);
}

void Platform::physicsUpdate(float dt)
{
	m_time += dt;

	if (m_state & PlatformState_HMove)
		m_velocity = Vec2(cos(m_time + m_f) * m_apmlitude, m_velocity.y);

	if (m_state & PlatformState_VMove)
		m_velocity = Vec2(m_velocity.x, sin(m_time + m_f)* m_apmlitude);

	GameObject::physicsUpdate(dt);
}

void Platform::intersect(Collision* coll)
{
	if (coll->collisionWith(GameObjectType_Platform, GameObjectType_Platform))
		coll->m_needToResolve = false;

	GameObject::intersect(coll);
}

void Platform::collisionEnter(Collision* collision)
{
	intersect(collision);
	//std::cout << "Enter ";
	GameObject::collisionEnter(collision);
}

void Platform::collisionStay(Collision* collision)
{
	intersect(collision);
	//std::cout << "Stay ";
	GameObject::collisionStay(collision);
}

void Platform::collisionExit(Collision* collision)
{
	//std::cout << "Exit ";
	GameObject::collisionExit(collision);
}