/////////////////////////////////////////////////
// Include
#include "controller.h"
#include "pool.h"
#include "game.h"
#include "utils.h"
#include "platform.h"
#include <iostream>

/////////////////////////////////////////////////
// Variables
int maxYobjectNum = 0;

/////////////////////////////////////////////////
// Class Controller
Controller::Controller()
{
	m_game = 0;
	m_pool = 0;
}

void Controller::initialize(Game* game, Pool* pool, Camera* camera)
{
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		m_platformList[i] = 0;
	}

	maxYobjectNum = 0;

	m_pool = pool;
	m_game = game;
	m_camera = camera;
	loadLevel();
}

void Controller::loadLevel()
{
	if(m_game->getGameMode() == GameMode_OnePlayer)
	{
		m_platformList[0] = (Platform*)m_pool->getNewObject();
		m_platformList[0]->setPosition(Vec2(0, 175));
		m_platformList[0]->setActive(true);
	}

	if (m_game->getGameMode() == GameMode_TwoPlayers)
	{
		m_platformList[0] = (Platform*)m_pool->getNewObject();
		m_platformList[0]->setPosition(Vec2(-100, 175));
		m_platformList[0]->setActive(true);

		m_platformList[1] = (Platform*)m_pool->getNewObject();
		m_platformList[1]->setPosition(Vec2(100, 175));
		m_platformList[1]->setActive(true);
	}
}

void Controller::update(float dt)
{
	for(int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_platformList[i] != 0)
		{
			if ( m_camera->getPosition().y - m_platformList[i]->getPosition().y < -(kPixlelsInColumn * 1.5) || !m_platformList[i]->getActive())
			{
				m_pool->deleteObject(m_platformList[i]);
				m_platformList[i] = 0;
			}
			else if (m_platformList[i]->getStartPosition().y < m_platformList[maxYobjectNum]->getStartPosition().y)
			{
				maxYobjectNum = i;
			}
		}
	}

	if (abs(m_platformList[maxYobjectNum]->getPosition().y - m_camera->getPosition().y) < kPixlelsInColumn * 2)
	{
		createNextPlatforms(m_platformList[maxYobjectNum]->getPosition());
	}
}

void Controller::createNextPlatforms(Vec2 currentPlatformPos)
{
	Vec2 offset;

	int p = 0, t = GetRandomInt(0, 10);
	Vec2 maxPosition = Vec2(0, 250);

	if (t > 7) p = 3;
	else p = 2;

	float magnitude = 240;
	
	for (int j = 0; j < p; j++)
	{
		offset.y = -GetRandomFloat(magnitude / 3, magnitude);
		offset.x = GetRandomFloat( -magnitude + 2*magnitude/p * j, - magnitude + 2*magnitude/p * (j+1) );

		offset = offset.magnitude(magnitude);

		Vec2 newPosition = currentPlatformPos + offset;

		if (newPosition.x < -kPixlelsInRow / 2 - 32) newPosition.x = (kPixlelsInRow / 2 + 32) + newPosition.x - (-kPixlelsInRow / 2 - 32);
		if (newPosition.x > kPixlelsInRow / 2 - 32) newPosition.x = (-kPixlelsInRow / 2 - 32) + newPosition.x - (kPixlelsInRow / 2 + 32);

		int i = 0;
		while (m_platformList[i] != 0) i++;

		m_platformList[i] = (Platform*) m_pool->getNewObject();
		m_platformList[i]->setPosition(newPosition);
		m_platformList[i]->initialize();

		if (newPosition.y < maxPosition.y) maxPosition = newPosition;
	}
	if (abs(maxPosition.y - m_camera->getPosition().y) < kPixlelsInColumn * 2) createNextPlatforms(maxPosition);
}