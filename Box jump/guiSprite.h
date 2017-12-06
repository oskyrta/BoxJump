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
	~GUISprite();

	virtual void render();

	void setRect(sf::IntRect rect) { m_rect = rect; m_sprite->setTextureRect(rect); };
protected:
	sf::Sprite* m_sprite;
	sf::IntRect m_rect;
};