#pragma once
///////////////////////////////////////////////////
// Include
#include "gameObject.h"

///////////////////////////////////////////////////
// Class Cloud
class Cloud : public GameObject
{
public:
	Cloud();

	void update(float dt);

private:
	float m_cloudVelocity;
};