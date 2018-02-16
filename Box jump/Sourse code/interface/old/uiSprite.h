#pragma once
/////////////////////////////////////////////////
// Include
#include "guiObject.h"
#include <SFML\Graphics.hpp>

/////////////////////////////////////////////////
// Class GUISprite
class GUISprite : public virtual GUIObject
{
public:
	GUISprite(Camera* camera, std::string tag, Vec2 halfSize = Vec2()) 
		: GUIObject(camera, tag, halfSize) 
	{
		// Initialize sprite
		extern sf::Texture* g_atlas00;

		m_sprite = new sf::Sprite();
		m_sprite->setTexture(*g_atlas00);
		m_sprite->setTextureRect(sf::IntRect());
		m_spriteSize = 1;
	};
	~GUISprite();

	void render();

	void initialize(std::string spriteName);

	void setRect(sf::IntRect rect) { m_rect = rect; m_sprite->setTextureRect(rect); };
	void setScale(float scale) { m_sprite->setScale(scale, scale); };
	void setHalfSizeAutomatically() { m_halfSize = Vec2(m_rect.width * m_spriteSize, m_rect.height * m_spriteSize) / 2; }
protected:
	sf::Sprite* m_sprite;
	sf::IntRect m_rect;
	int m_spriteSize;
};