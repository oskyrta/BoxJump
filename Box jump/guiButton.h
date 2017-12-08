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
	GUIButton(Camera* camera, std::string tag, Vec2 halfSize)
		: GUISprite(camera, tag, halfSize)
		, GUIText(camera, tag, halfSize)
		, GUIObject(camera, tag, halfSize)
	{
		m_eventController = 0;
		m_event = GameEvent_None;
	};

	virtual void render();
	virtual void update();

	void initialize(EventController* eventController, GameEvents gameEvent);

	void setEvent(GameEvents gameEvent) { m_event = gameEvent; };
	void setEventController(EventController* controller) { m_eventController = controller; };
	void setSprite(std::string spriteName) { GUISprite::initialize(spriteName); GUISprite::setHalfSizeAutomatically(); };

private:
	GameEvents m_event;
	EventController* m_eventController;
};