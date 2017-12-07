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
	GUISprite();
	GUISprite(std::string tag, Vec2 halfSize, Camera* camera) : GUIObject(tag, halfSize, camera) {};
	~GUISprite();

	void render();

	void initialize(Vec2 pos);

	void setRect(sf::IntRect rect) { m_rect = rect; m_sprite->setTextureRect(rect); };
	void setScale(float scale) { m_sprite->setScale(scale, scale); };
	void setPosition(Vec2 pos) { m_pos = pos; }
protected:
	sf::Sprite* m_sprite;
	sf::IntRect m_rect;
};