/////////////////////////////////////////////////
// Include
#include "guiButton.h"
#include "eventController.h"
#include "levelSettings.h"
#include "utils.h"
#include <Windows.h>

/////////////////////////////////////////////////
// Variable 
bool leftClick = false;

/////////////////////////////////////////////////
// Class GUIButton
GUIButton::GUIButton()
{
	m_eventController = 0;
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
	GUISprite::render();
	GUIText::render();
}