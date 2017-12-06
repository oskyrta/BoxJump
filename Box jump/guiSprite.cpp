////////////////////////////////////////////////
// Include
#include "guiSprite.h"
#include "camera.h"

////////////////////////////////////////////////
// Extern declaration
extern sf::Texture* g_atlas00;

////////////////////////////////////////////////
// Class GUISprite
GUISprite::GUISprite()
{
	// Initialize sprite
	m_sprite = new sf::Sprite();
	m_sprite->setTexture(*g_atlas00);
	m_sprite->setTextureRect(sf::IntRect());
}

GUISprite::~GUISprite()
{
	if (m_sprite)
		delete m_sprite;
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