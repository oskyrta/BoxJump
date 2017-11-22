////////////////////////////////////////////////
// Include
#include "collision.h"

////////////////////////////////////////////////
// Class Collision
Collision::Collision()
{
	clear();
}

bool ObjectsPair::operator==(const ObjectsPair& a)
{
	if (object1 == a.object1 && object2 == a.object2) return true;
	if (object1 == a.object2 && object2 == a.object1) return true;
	return false;
}

void Collision::clear()
{
	m_objects.object1 = 0;
	m_objects.object2 = 0;

	m_normal = Vec2();
	m_enterDirection = Vec2();

	m_state = CollisionState_Start;

	m_depth = 0;

	m_needToResolve = true;
}