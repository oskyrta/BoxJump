////////////////////////////////////////////////
// Include
#include "guiSprite.h"
#include "camera.h"
#include "settingsManager.h"

////////////////////////////////////////////////
// Extern declaration
extern sf::Texture* g_atlas00;
extern SettingsManager settingsManager;

////////////////////////////////////////////////
// Class GUISprite
GUISprite::~GUISprite()
{
	if (m_sprite)
		delete m_sprite;
}

void GUISprite::initialize(std::string spriteName)
{
	// Get sprite parameters from data
	sf::IntRect rect;
	rect.left = settingsManager.p_spriteParameters->get<int>(spriteName + ".x");
	rect.top = settingsManager.p_spriteParameters->get<int>(spriteName + ".y");
	rect.width = settingsManager.p_spriteParameters->get<int>(spriteName + ".width");
	rect.height = settingsManager.p_spriteParameters->get<int>(spriteName + ".height");
	m_spriteSize = settingsManager.p_spriteParameters->get<int>(spriteName + ".size");

	m_sprite->setTextureRect(rect);
	m_rect = rect;
	m_sprite->setScale(m_spriteSize, m_spriteSize);
}

void GUISprite::render()
{
	if (m_rect.height != 0)
	{
		// Calculation top left point
		float x = (m_pos.x - m_sprite->getTextureRect().width / 2);
		float y = (m_pos.y - m_sprite->getTextureRect().height / 2);

		// Set position relative to the camera
		m_sprite->setPosition(roundf(x) + m_camera->getPosition().x, roundf(y) + m_camera->getPosition().y);

		m_camera->getRenderWindow()->draw(*m_sprite);
	}
}