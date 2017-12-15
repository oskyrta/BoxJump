//////////////////////////////////////////////////
// Incude
#include "cloud.h"
#include "settingsManager.h"

//////////////////////////////////////////////////
// Extern declaration
extern SettingsManager settingsManager;

//////////////////////////////////////////////////
//Class Cloud
Cloud::Cloud()
{
	m_type = GameObjectType_Cloud;
	m_isRigid = false;
	m_useGravity = false;
	m_cloudVelocity = settingsManager.p_objectsSettings->get<float>("Cloud.CloudSpeed");
	m_velocity = Vec2(m_cloudVelocity, 0);
}

void Cloud::update(float dt)
{

	GameObject::update(dt);
}