//////////////////////////////////////////////////
// Incude
#include "gameObject\cloud.h"
#include"dataManager\dataManager.h"

//////////////////////////////////////////////////
// Extern declaration

//////////////////////////////////////////////////
//Class Cloud
Cloud::Cloud()
{
	m_type = GameObjectType_Cloud;
	m_isRigid = false;
	m_useGravity = false;
	m_cloudVelocity = DataManager::instance()->getObjectsSettgins()->get<float>("Cloud.CloudSpeed");
	m_velocity = Vec2(m_cloudVelocity + (rand()%14 - 7), 0);
}

void Cloud::update(float dt)
{

	//GameObject::update(dt);
}