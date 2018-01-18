/////////////////////////////////////////////////
// Include
#include "controller.h"
#include "pool.h"
#include "game.h"
#include "utils.h"
#include "platform.h"
#include "cloud.h"
#include <iostream>

/////////////////////////////////////////////////
// Variables
int maxYplatformIndex = 0;
int maxYcloudIndex = 2;

/////////////////////////////////////////////////
// Class Controller
Controller::Controller()
{
	m_game = 0;
	m_platformPool = 0;
	m_cloudPool = 0;
}

void Controller::initialize(Game* game, Pool* platformPool, Pool* cloudPool, Camera* camera)
{
	for (int i = 0; i < kMaxObjectsCount; i++)
	{
		m_objectsList[i] = 0;
	}

	maxYplatformIndex = 0;
	maxYcloudIndex = 2;

	m_platformPool = platformPool;
	m_cloudPool = cloudPool;
	m_game = game;
	m_camera = camera;
	loadLevel();
}

void Controller::loadLevel()
{
	// Create platforms under the players
	if(m_game->getGameMode() == GameMode_OnePlayer)
	{
		m_objectsList[0] = m_platformPool->getNewObject();
		m_objectsList[0]->setPosition(Vec2(0, 175));
		m_objectsList[0]->setActive(true);
	}

	if (m_game->getGameMode() == GameMode_TwoPlayers)
	{
		m_objectsList[0] = m_platformPool->getNewObject();
		m_objectsList[0]->setPosition(Vec2(-100, 175));
		m_objectsList[0]->setActive(true);

		m_objectsList[1] = m_platformPool->getNewObject();
		m_objectsList[1]->setPosition(Vec2(100, 175));
		m_objectsList[1]->setActive(true);
	}

	m_objectsList[2] = m_cloudPool->getNewObject();
	m_objectsList[2]->setPosition(Vec2(-200, 2));
	m_objectsList[2]->setActive(true);
}

void Controller::update(float dt)
{
	for(int i = 0; i < kMaxObjectsCount; i++)
	{
		if (m_objectsList[i] != 0)
		{
			if (m_objectsList[i]->getType() == GameObjectType_Platform)
			{
				Platform* platform = (Platform*)m_objectsList[i];
				if (m_camera->getPosition().y - platform->getPosition().y < -(kPixlelsInColumn * 1.5) || !platform->getActive())
				{
					m_platformPool->deleteObject(m_objectsList[i]);
					m_objectsList[i] = 0;
				}
				else if (platform->getStartPosition().y < ((Platform*)m_objectsList[maxYplatformIndex])->getStartPosition().y)
				{
					maxYplatformIndex = i;
				}
			}
			else if (m_objectsList[i]->getType() == GameObjectType_Cloud)
			{
				Cloud* cloud = (Cloud*)m_objectsList[i];
				if (cloud->getPosition().x - m_camera->getPosition().x > m_camera->getSize().x / 1.8f)
					cloud->addToPosition(Vec2(-m_camera->getSize().x - GetRandomFloat(60, 100), GetRandomFloat(-30, 30)));

				if (cloud->getPosition().y - m_camera->getPosition().y > kPixlelsInColumn || !cloud->getActive())
				{
					m_platformPool->deleteObject(m_objectsList[i]);
					m_objectsList[i] = 0;
				}
				else if (cloud->getPosition().y < m_objectsList[maxYcloudIndex]->getPosition().y)
				{
					maxYcloudIndex = i;
				}
			}
		}
	}

	if (m_camera->getPosition().y - m_objectsList[maxYcloudIndex]->getPosition().y < kPixlelsInColumn/2)
	{
		createNextCloud(m_objectsList[maxYcloudIndex]->getPosition());
	}

	if (abs(m_objectsList[maxYplatformIndex]->getPosition().y - m_camera->getPosition().y) < kPixlelsInColumn * 2)
	{
		createNextPlatforms(m_objectsList[maxYplatformIndex]->getPosition());
	}
}

void Controller::createNextCloud(Vec2 cloudPos)
{
	int i = 0;
	while (m_objectsList[i] != 0) i++;

	cloudPos = Vec2(-m_camera->getSize().x / 1.7 - GetRandomFloat(60, 100), cloudPos.y - GetRandomFloat(100, 300));

	m_objectsList[i] = m_cloudPool->getNewObject();
	m_objectsList[i]->setPosition(cloudPos);

	sf::IntRect rect = m_objectsList[i]->getTextureRect();
	rect.top = GetRandomInt(0, 2) * 16;
	m_objectsList[i]->setTextureRect(rect);

	if (m_objectsList[i]->getDepth() != 2) m_objectsList[i]->setDepth(2);
	m_objectsList[i]->setActive(true);
	maxYcloudIndex = i; 

	if (m_camera->getPosition().y - m_objectsList[maxYcloudIndex]->getPosition().y < kPixlelsInColumn / 2)
	{
		createNextCloud(m_objectsList[maxYcloudIndex]->getPosition());
	}
}

void Controller::createNextPlatforms(Vec2 currentPlatformPos)
{
	Vec2 offset;

	int p = 0, t = GetRandomInt(0, 10);
	Vec2 maxPosition = Vec2(0, 250);

	if (t > 8) p = 3;
	else if(t > 4) p = 2;
	else p = 1;

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
		while (m_objectsList[i] != 0) i++;

		m_objectsList[i] = (Platform*) m_platformPool->getNewObject();
		m_objectsList[i]->setPosition(newPosition);
		if (m_objectsList[i]->getDepth() != 1) m_objectsList[i]->setDepth(1);
		((Platform*)m_objectsList[i])->initialize();

		if (newPosition.y < maxPosition.y) maxPosition = newPosition;
	}
	if (abs(maxPosition.y - m_camera->getPosition().y) < kPixlelsInColumn * 2) createNextPlatforms(maxPosition);
}