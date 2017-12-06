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

	// Get mouse position on screen
	sf::Vector2i mp = sf::Mouse::getPosition(*m_camera->getRenderWindow());
	// Calculate mouse position in world coordinate considering zoom
	sf::Vector2f worldPosition = m_camera->getRenderWindow()->mapPixelToCoords(mp);
	// Calculate position on game interface
	Vec2 mousePosition = Vec2(worldPosition.x, worldPosition.y) - m_camera->getPosition();

	// Check intersect with mouse
	if (abs(mousePosition.x - m_pos.x) <= m_halfSize.x && abs(mousePosition.y - m_pos.y) <= m_halfSize.y) m_mouseOnObject = true;
}