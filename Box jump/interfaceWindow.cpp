////////////////////////////////////////////////
// Include
#include "interfaceWindow.h"
#include "camera.h"
#include "guiButton.h"
#include "guiText.h"
#include "guiObject.h"
#include "guiSprite.h"
#include "eventController.h"
#include "settingsManager.h"
#include "utils.h"
#include <string>

////////////////////////////////////////////////
// Extern
extern sf::Font* g_font;
extern SettingsManager settingsManager;

////////////////////////////////////////////////
// Class InterfaceWindow
InterfaceWindow::InterfaceWindow()
{
	m_isActive = false;

	// Clear buttons ans text lists
	for (int i = 0; i < 30; i++)
	{
		m_objectsList[i] = 0;
	}
}

InterfaceWindow::~InterfaceWindow()
{
	// Delete interface objects
	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] != 0)
		{
			delete m_objectsList[i];
			m_objectsList[i] = 0;
		}
	}
}

GUIButton* InterfaceWindow::addButton(std::string name, Vec2 halfSize, std::string tag, GameEvents gameEvent, int characterSize)
{
	GUIButton* button = 0;
	//Found free space for button
	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] == 0)
		{
			button = new GUIButton(m_camera, tag, halfSize);
			m_objectsList[i] = button;
			break;
		}
	}

	// Exit if free spce not available
	if (button == 0) return button;

	// Initialize button
	button->initialize(m_eventController, gameEvent);
	button->GUIText::initialize(name, g_font, characterSize);

	return button;
}

GUIText* InterfaceWindow::addText(std::string name, Vec2 halfSize, std::string tag, int characterSize)
{
	GUIText* text = 0;
	// Found free space for text
	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] == 0)
		{
			text = new GUIText(m_camera, tag, halfSize);
			m_objectsList[i] = text;
			break;
		}
	}
	// Exit if free space not available
	if (text == 0) return text;

	// Initialize text
	text->initialize(name, g_font, characterSize);

	return text;
}

GUISprite* InterfaceWindow::addSprite(std::string spriteName, std::string tag)
{
	GUISprite* sprite = 0; 

	// Found free space for sprite
	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] == 0)
		{
			sprite = new GUISprite(m_camera, tag);
			m_objectsList[i] = sprite;
			break;
		}
	}
	// Exit if free space not available
	if (sprite == 0) return sprite;
	
	sprite->initialize(spriteName);
	sprite->setHalfSizeAutomatically();
}

void InterfaceWindow::update()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_camera->getRenderWindow());
	sf::Vector2f worldPosition = m_camera->getRenderWindow()->mapPixelToCoords(mousePosition);

	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] != 0)
			m_objectsList[i]->update();
	}
	
	if (IsKeyDown(0x70) && m_eventController->getEventState(GameEvent_LeftButtonStay))
	{
		for (int i = 0; i < 30; i++)
		{
			if (m_objectsList[i] != 0 && m_objectsList[i]->getMouseOnObject() && m_eventController->getEventState(GameEvent_LeftButtonStay) )
			{
				// Calculate cursor position on window
				Vec2 windowCursorPosition = Vec2(round(worldPosition.x), round(worldPosition.y)) + m_camera->getPosition();
				m_objectsList[i]->setPosition(windowCursorPosition);

				break;
			}
		}
	}
}

void InterfaceWindow::render()
{
	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] != 0)
		{
			m_objectsList[i]->render();
		}
	}
}