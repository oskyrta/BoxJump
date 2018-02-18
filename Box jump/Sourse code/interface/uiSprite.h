#pragma once
/////////////////////////////////////////////////
// Include
#include "uiObject.h"
#include <SFML\Graphics.hpp>

struct SpriteContainer;

/////////////////////////////////////////////////
// Class GUISprite
class UISprite : public UIObject
{
public:
	UISprite();
	~UISprite();

	void render();
	void update();

	void initialize(PropertyTree* tree);

	void setRect(sf::IntRect & rect) { m_rect = rect; m_sprite->setTextureRect(rect); };
	sf::IntRect getRect() { return m_rect; }
	void setScale(float scale) { m_sprite->setScale(scale, scale); };
	void resetSprite(std::string name);

protected:
	std::string m_spriteName;

	SpriteContainer* m_spriteContainer;
	sf::Sprite* m_sprite;
	sf::IntRect m_rect;
	int m_spriteSize;
};