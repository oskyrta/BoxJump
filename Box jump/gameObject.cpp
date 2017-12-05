////////////////////////////////////////////////
// Include
#include "gameObject.h"
#include "levelSettings.h"
#include "game.h"
#include "collision.h"

#include <iostream>

////////////////////////////////////////////////
// Extern
extern sf::Texture* g_atlas00;

////////////////////////////////////////////////
// Declaration

////////////////////////////////////////////////
// Class GameObject

GameObject::GameObject()
{
	m_position = Vec2(0, 0);
	m_velocity = Vec2(0, 0);
	m_forse = Vec2(0, 0);
	m_newPosition = Vec2(0, 0);

	m_game = 0;
	m_camera = 0;

	m_isActive = true;
	
	m_isRigid = true;
	m_useGravity = false;

	sprintf_s(m_name, 20, "Object");

	m_massData.mass = 0;
	m_massData.inv_mass = 0;

	m_material.density = 0;
	m_material.restitution = 0;
	m_material.staticFriction = 0;
	m_material.dynamicFriction = 0;

	for (int i = 0; i < 32; i++)
	{
		m_polygon.vertex[i] = Vec2(0, 0);
		m_normals[i] = Vec2(0, 0);
	}

	m_polygon.position = &m_position;

	// Initialize sprite
	m_sprite = new sf::Sprite();
	m_sprite->setTexture(*g_atlas00);
	m_sprite->setTextureRect(sf::IntRect());
}

GameObject::~GameObject()
{
	if (m_sprite)
		delete m_sprite;
}

void GameObject::render(float alpha)
{
	if (!m_isActive)
		return;

	// Interpolate position between physics update
	Vec2 pos = m_newPosition * alpha + m_position * (1.0 - alpha);
	// Don't interpolate if object teleported
	if (abs(m_newPosition.x - pos.x) > 16 || abs(m_newPosition.y - pos.y) > 16)
		pos = m_newPosition;

	// Calculation top left point
	pos = pos - m_size / 2;

	// Snap to pixels grid
	//x -= (int)x % m_camera->getPixelSize();
	//y -= (int)y % m_camera->getPixelSize();

	m_sprite->setPosition(lroundf(pos.x), lroundf(pos.y));

	m_camera->getRenderWindow()->draw(*m_sprite);

	// Draw collider
	 /*
	for (int i = 0; i < m_polygon.vertexCount; i++)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f((m_polygon.vertex[i] + m_position).x, (m_polygon.vertex[i] + m_position).y)),
			sf::Vertex(sf::Vector2f((m_polygon.vertex[(i + 1) % m_polygon.vertexCount] + m_position).x, (m_polygon.vertex[(i + 1) % m_polygon.vertexCount] + m_position).y))
		};
		line->color = sf::Color::Red;
		(line+1)->color = sf::Color::Red;
		m_camera->getRenderWindow()->draw(line, 2, sf::Lines);
	}
	// */

	// Draw normals
	 /*
	for (int i = 0; i < m_polygon.vertexCount; i++)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(m_position.x, m_position.y)),
			sf::Vertex(sf::Vector2f((m_normals[i] * 20 + m_position).x , (m_normals[i] * 20 + m_position).y ))
		};
		m_camera->getRenderWindow()->draw(line, 2, sf::Lines);
	} // */
	
}

void GameObject::update(float dt)
{
	if (m_newPosition.x - m_size.x / 2 > kPixlelsInColumn / 2) m_newPosition.x = -kPixlelsInColumn / 2 - m_size.x / 2;
	if (m_newPosition.x + m_size.x / 2 < -kPixlelsInColumn / 2) m_newPosition.x = kPixlelsInColumn / 2 + m_size.x / 2;
}

void GameObject::physicsUpdate(float dt)
{
	m_velocity += m_forse * m_massData.inv_mass * 0.5f * dt;
	m_position = m_newPosition;
	m_newPosition += m_velocity * dt;
	m_velocity += m_forse * m_massData.inv_mass * 0.5f * dt;

	m_forse = Vec2();

}

void GameObject::intersect(Collision* collision)
{
	
}

void GameObject::collisionEnter(Collision* collision)
{
	//std::cout << "Enter ";
}

void GameObject::collisionStay(Collision* collision)
{
	//std::cout << "Stay ";
}

void GameObject::collisionExit(Collision* collision)
{
	//std::cout << "Exit ";
}

void GameObject::move()
{
	
}

void GameObject::setColliderVertex(int count, Vec2* vertexes)
{
	m_polygon.vertexCount = count;
	for (int i = 0; i < count; i++)
	{
		m_polygon.vertex[i] = vertexes[i];
	}

	for (int i = 0; i < count; i++)
	{
		m_normals[i] = Vec2((m_polygon.vertex[(i + 1) % count].y - m_polygon.vertex[i].y), -(m_polygon.vertex[(i + 1) % count].x - m_polygon.vertex[i].x));
	}
}

void GameObject::calculateVertexAutomatically()
{
	float halfWidth = m_size.x / (2);
	float halfHeight = m_size.y / (2);
	Vec2 ver[4] = { Vec2(-halfWidth, -halfHeight), Vec2(halfWidth, -halfHeight), Vec2(halfWidth,  halfHeight), Vec2(-halfWidth,  halfHeight)};

	setColliderVertex(4, ver);
}