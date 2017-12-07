#pragma once
////////////////////////////////////////////////
// Include
#include <string>
#include <SFML\Graphics.hpp>
#include "vec2.h"

////////////////////////////////////////////////
// Forvard declaration
class Camera;
class GUIObject;
class GUISprite;
class GUIText;
class GUIButton;
class EventController;
enum GameEvents;

////////////////////////////////////////////////
// Class InterfaceWindow
class InterfaceWindow
{
public:
	InterfaceWindow();
	~InterfaceWindow();

	void update();
	void render();

	GUIButton* addButton( std::string name, Vec2 size, std::string tag, GameEvents gameEvent, int characterSize = 16);
	GUIText* addText(std::string name, Vec2 halfSize, std::string tag, int characterSize = 16);
	GUISprite* addSprite(std::string spriteName, std::string tag);

	void setActive(bool active) { m_isActive = active; };
	bool getActive() { return m_isActive; };

	void setEventController(EventController* ec) { m_eventController = ec; };
	void setCamera(Camera* camera) { m_camera = camera; };

private:
	bool m_isActive;

	EventController* m_eventController;

	GUIObject* m_objectsList[30];

	Camera* m_camera;
};