/////////////////////////////////////////////////
// Include
#include "tools\pool.h"
#include <iostream>

/////////////////////////////////////////////////
// Class Pool
Pool::Pool()
{
	m_objectsCount = 0;

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		m_poolObjects[i] = 0;
	}
}

void Pool::setup(Game* game, GameObjectType type, int count)
{
	m_game = game;
	m_type = type;

	m_objectsCount = 0;

	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		m_poolObjects[i] = 0;
	}

	for (int i = 0; i < count; i++)
	{
		m_poolObjects[m_objectsCount] = m_game->createGameObject(type, 0, 0);
		m_poolObjects[m_objectsCount]->setActive(false);

		m_objectsCount++;
	}
}

GameObject* Pool::getNewObject()
{
	for (int i = 0; i < m_objectsCount; i++)
	{
		if (m_poolObjects[i]->getActive() == false)
		{
			return m_poolObjects[i];
		}
	}

	m_poolObjects[m_objectsCount] = m_game->createGameObject(m_type, 0, 0);
	m_poolObjects[m_objectsCount]->setActive(false);

	m_objectsCount++;

	return m_poolObjects[m_objectsCount - 1];
}

void Pool::deleteObject(GameObject* object)
{
	for (int i = 0; i < m_objectsCount; i++)
	{
		if (object == m_poolObjects[i])
		{
			m_poolObjects[i]->setActive(false);
			return;
		}
	}
}