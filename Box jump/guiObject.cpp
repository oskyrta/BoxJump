//////////////////////////////////////////////
// Include
#include "guiObject.h"
#include <SFML\Graphics.hpp>
#include "camera.h"
#include "settingsManager.h"
#include "utils.h"

//////////////////////////////////////////////
// Extern 
extern SettingsManager settingsManager;

//////////////////////////////////////////////
// Class GUIObject
GUIObject::GUIObject()
{
	m_pos = Vec2();
	m_halfSize = Vec2();
	m_tag = "";
	m_camera = 0;
}

GUIObject::GUIObject(Camera* camera, std::string tag, Vec2 halfSize)
{
	m_pos = GetPositionByTag(tag);
	m_halfSize = halfSize;
	m_camera = camera;
	m_tag = tag;
}

GUIObject::~GUIObject()
{
	//std::cout << m_tag + " " << m_pos.x << " " << m_pos.y << std::endl;
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