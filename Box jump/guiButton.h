#pragma once
////////////////////////////////////////////////
// Include
#include "guiText.h"

////////////////////////////////////////////////
// Forvard declaration
class EventController;
enum GameEvents;

////////////////////////////////////////////////
// Class GUIButton
class GUIButton : public GUIText
{
public:
	GUIButton();

	virtual void render();
	virtual void update();

	void setRect(sf::IntRect rect) { m_rect = rect; m_sprite->setTextureRect(rect);};
	void setEvent(GameEvents gameEvent) { m_event = gameEvent; };
	void setEventController(EventController* controller) { m_eventController = controller; };

private:
	GameEvents m_event;
	EventController* m_eventController;

	sf::Sprite* m_sprite;
	sf::IntRect m_rect;
};