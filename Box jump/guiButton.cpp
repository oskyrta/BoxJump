/////////////////////////////////////////////////
// Include
#include "guiButton.h"
#include "eventController.h"
#include "camera.h"
#include "levelSettings.h"
#include "utils.h"
#include <Windows.h>

/////////////////////////////////////////////////
// Variable 
bool leftClick = false;

////////////////////////////////////////////////
// Extern
extern sf::Texture* g_atlas00;

/////////////////////////////////////////////////
// Class GUIButton
GUIButton::GUIButton()
{
	m_eventController = 0;

	// Initialize sprite
	m_sprite = new sf::Sprite();
	m_sprite->setTexture(*g_atlas00);
	m_sprite->setTextureRect(sf::IntRect());
}

void GUIButton::update()
{
	GUIObject::update();

	leftClick = false;
	if (m_eventController->getEventState(GameEvent_LeftButtonDown)) leftClick = true;

	if (m_mouseOnObject && !IsKeyDown(0x70)) m_textOffset.x = 8;
	else m_textOffset.x = 0;

	if (leftClick && m_mouseOnObject && !IsKeyDown(0x70))
	{
		m_eventController->startEvent(m_event);
	}
}

void GUIButton::render()
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

	GUIText::render();
}