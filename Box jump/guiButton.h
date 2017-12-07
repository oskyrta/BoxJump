#pragma once
////////////////////////////////////////////////
// Include
#include "guiText.h"
#include "guiSprite.h"

////////////////////////////////////////////////
// Forvard declaration
class EventController;
enum GameEvents;

////////////////////////////////////////////////
// Class GUIButton
class GUIButton : public GUIText, public GUISprite
{
public:
	GUIButton();
	GUIButton(std::string tag, Vec2 halfSize, Camera* camera) : GUIText(tag, halfSize, camera), GUISprite(tag, halfSize, camera) {};

	virtual void render();
	virtual void update();

	void initialize(std::string, sf::Font* font, int characterSize);

	void setEvent(GameEvents gameEvent) { m_event = gameEvent; };
	void setEventController(EventController* controller) { m_eventController = controller; };

private:
	GameEvents m_event;
	EventController* m_eventController;
};