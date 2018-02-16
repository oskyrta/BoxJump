#pragma once
////////////////////////////////////////////////
// Include
#include "uiText.h"
#include "uiSprite.h"

////////////////////////////////////////////////
// Forvard declaration
class EventController;

////////////////////////////////////////////////
// Class GUIButton
class UIButton : public UIText
{
public:
	UIButton();
	~UIButton();

	//////// Button  //////////
	virtual void render();
	virtual void update();

	void initialize(PropertyTree* tree);

	void setEvent(std::string gameEvent) { m_event = gameEvent; };

	////////  Sprite  /////////
	void setRect(sf::IntRect rect) { m_rect = rect; m_sprite->setTextureRect(rect); };
	void setScale(float scale) { m_sprite->setScale(scale, scale); };
	void setSpriteName(std::string name) { m_spriteName = name; }

private:
	void initializeSprite();

private:
	std::string m_event;
	EventController* m_eventController;

	////////  Sprite  /////////
	std::string m_spriteName;

	SpriteContainer* m_spriteContainer;
	sf::Sprite* m_sprite;
	sf::IntRect m_rect;
	int m_spriteSize;
};