////////////////////////////////////////////////
// Include
#include "interfaceWindow.h"
#include "camera.h"
#include "guiButton.h"
#include "guiText.h"
#include "guiObject.h"
#include "eventController.h"
#include "settingsManager.h"
#include "utils.h"
#include <string>

////////////////////////////////////////////////
// Extern
extern sf::Font* g_font;
extern SettingsManager settingsManager;

Vec2 getPositionFromData(std::string tag);

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
			button = new GUIButton();
			m_objectsList[i] = button;
			break;
		}
	}

	// Exit if free spce not available
	if (button == 0) return button;

	// Initialize button
	button->setCamera(m_camera);
	button->initialize(name, getPositionFromData(tag), halfSize, g_font, characterSize);
	button->setEvent(gameEvent);
	button->setEventController(m_eventController);
	button->setTag(tag);

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
			text = new GUIText();
			m_objectsList[i] = text;
			break;
		}
	}
	// Exit if free space not available
	if (text == 0) return text;

	// Initialize text
	text->setCamera(m_camera);
	text->initialize(name, getPositionFromData(tag), halfSize, g_font, characterSize);
	text->setTag(tag);

	return text;
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
				Vec2 windowCursorPosition = Vec2(worldPosition.x, worldPosition.y) - m_camera->getPosition();
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

Vec2 getPositionFromData(std::string tag)
{
	return Vec2(settingsManager.p_interfaceSettings->get<int>(tag + ".x", 0), settingsManager.p_interfaceSettings->get<int>(tag + ".y", 0));
}