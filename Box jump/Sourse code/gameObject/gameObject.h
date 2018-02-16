#pragma once
////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>
#include "gameObject/gameObjectType.h"
#include "tools\vec2.h"
#include "render\camera.h"
#include "game.h"

////////////////////////////////////////////////
// Struct
struct ObjectPolygon
{
	Vec2 vertex[32];
	int vertexCount = 0;
	const Vec2* position;
};

struct MassData
{
	float mass;
	float inv_mass;
};

struct Material
{
	float density;
	float restitution;

	float staticFriction;
	float dynamicFriction;
};

////////////////////////////////////////////////
// Class GameObject
class GameObject 
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void render(float alpha);
	virtual void update(float dt);
	virtual void physicsUpdate(float dt);

	virtual void intersect(Collision* collision);
	virtual void collisionEnter(Collision* collision);
	virtual void collisionStay(Collision* collision);
	virtual void collisionExit(Collision* collision);

	void move();

	void setPosition(Vec2 position) { m_position = position; m_newPosition = position; };
	Vec2 getPosition() { return m_position; };

	void setNewPosition(Vec2 pos) { m_newPosition = pos; };
	Vec2 getNewPosition() { return m_newPosition; };

	void setActive(bool isActive) { m_isActive = isActive; };
	bool getActive() { return m_isActive; };

	void setCamera(Camera* camera) { m_camera = camera; };

	void setScale(float scale) { m_sprite->setScale(scale, scale); };

	void setMass(float mass) { m_massData.mass = mass; m_massData.inv_mass = (mass > 0) ? 1.0f/mass : 0; };
	MassData getMass() { return m_massData; };

	void setMaterial(float density, float restitution) { m_material.density = density; m_material.restitution = restitution; };
	Material getMaterial() { return m_material; };

	void setVelocity(Vec2 velocity){ m_velocity = velocity; };

	Vec2 getVelocity() { return m_velocity; };

	void setSize(Vec2 size);
	Vec2 getSize() { return m_size; };

	void setUseGravity(bool useGravity) { m_useGravity = useGravity; };

	void setColliderVertex(int count, Vec2* vertexs);
	ObjectPolygon* getPolygon() { return &m_polygon; };
	void calculateVertexAutomatically();

	Vec2 getNormal(int num) { return m_normals[num]; };

	GameObjectType getType() { return m_type; };

	void setTextureRect(sf::IntRect rect) { m_sprite->setTextureRect(rect); };
	sf::IntRect getTextureRect() { return m_sprite->getTextureRect(); }

	void setTexture(sf::Texture & tex) { m_sprite->setTexture(tex); }

	void setName(char name[]) { sprintf_s(m_name, 32, name); };
	const char* getName() { return m_name; };

	void setRigid(bool rigid) { m_isRigid = rigid; };
	bool getRigid() { return m_isRigid; };

	void setDepth(float depth) { m_depth = depth; m_game->sortGameObjectsBeforeRender(); };
	float getDepth() { return m_depth;};

	bool objectUseGravity() { return m_useGravity; };

	void addForse(Vec2 forse) { m_forse += forse; };
	void addVelocity(Vec2 velocity) { m_velocity += velocity * m_massData.inv_mass; };
	void addToPosition(Vec2 pos) { m_newPosition += pos; };

	void calculateMass() { setMass(m_size.x * m_size.y * m_material.density); };

protected:
	GameObjectType m_type;

	Game* m_game;
	Camera* m_camera;

	Vec2 m_position;
	Vec2 m_newPosition;
	float m_depth;

	Vec2 m_velocity;
	Vec2 m_forse;

	Vec2 m_size;

	MassData m_massData;
	Material m_material;

	ObjectPolygon m_polygon;
	Vec2 m_normals[32];

	bool m_isActive;
	bool m_isMoveable;
	bool m_useGravity;
	bool m_isRigid;

	char m_name[32];

	sf::Sprite* m_sprite;
};