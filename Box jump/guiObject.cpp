//////////////////////////////////////////////
// Include
#include "guiObject.h"
#include <SFML\Graphics.hpp>
#include "camera.h"
#include "settingsManager.h"

//////////////////////////////////////////////
// Extern 
extern SettingsManager settingsManager;

//////////////////////////////////////////////
// Class GUIObject

GUIObject::~GUIObject()
{
	settingsManager.p_interfaceSettings->put(m_tag + ".x", m_pos.x);
	settingsManager.p_interfaceSettings->put(m_tag + ".y", m_pos.y);
}

void GUIObject::update()
{
	m_mouseOnObject = false;

	sf::Vector2i mp = sf::Mouse::getPosition(*m_camera->getRenderWindow());
	sf::Vector2f worldPosition = m_camera->getRenderWindow()->mapPixelToCoords(mp);
	Vec2 mousePosition = Vec2(worldPosition.x, worldPosition.y) - m_camera->getPosition();

	// Calculate cursor position on window
	Vec2 windowCursorPosition = mousePosition;// -m_camera->getSize() / 2;
	// Check intersect with mouse
	if (abs(mousePosition.x - m_pos.x) <= m_halfSize.x && abs(mousePosition.y - m_pos.y) <= m_halfSize.y) m_mouseOnObject = true;
}