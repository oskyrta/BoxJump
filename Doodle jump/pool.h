#pragma once
/////////////////////////////////////////////////
// Include
#include "gameObject.h"
#include "game.h"

/////////////////////////////////////////////////
// Class Pool
class Pool
{
public:
	Pool();
	~Pool();

	void setup(Game* game, GameObjectType type, int count);

	GameObject* getNewObject();
	void deleteObject(GameObject* object);

	void setGame(Game* game) { m_game = game; };

private:
	int m_objectsCount;
	GameObjectType m_type;

	Game* m_game;

	GameObject* m_poolObjects[kMaxObjectsCount];
};