////////////////////////////////////////////////
// Include
#include "collision.h"
#include "gameObject.h"

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

void Collision::sendToObjects()
{
	switch (m_state)
	{
		case CollisionState_Start: m_objects.object1->collisionEnter(this);
		case CollisionState_Stay: m_objects.object1->collisionStay(this);
		case CollisionState_Exit: m_objects.object1->collisionExit(this);
	}

	Collision reversedColl;
	reversedColl.setObjects(m_objects.object2, m_objects.object1);
	reversedColl.setNormal(m_normal * -1);
	reversedColl.setDepth(m_depth);
	reversedColl.setEnterDiretction(m_enterDirection * -1);
	reversedColl.setState(m_state);

	switch (m_state)
	{
		case CollisionState_Start: m_objects.object2->collisionEnter(&reversedColl);
		case CollisionState_Stay: m_objects.object2->collisionStay(&reversedColl);
		case CollisionState_Exit: m_objects.object2->collisionExit(&reversedColl);
	}

	if (!reversedColl.m_needToResolve) 
		m_needToResolve = reversedColl.m_needToResolve;
}

bool Collision::collisionWith(GameObjectType type1, GameObjectType type2)
{ 
	if (m_objects.object1->getType() == type1 && m_objects.object2->getType() == type2) return true;
	if (m_objects.object1->getType() == type2 && m_objects.object2->getType() == type1) return true;
	return false;
}