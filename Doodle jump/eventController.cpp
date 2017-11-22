////////////////////////////////////////////////
// Include
#include<SFML/Graphics.hpp>
#include "eventController.h"

////////////////////////////////////////////////
// Variable
bool LB_DownInLastFrame = false;

////////////////////////////////////////////////
// Class EventController
void EventController::update()
{
	if (m_events[GameEvent_LeftButtonDown] || m_events[GameEvent_LeftButtonStay]) LB_DownInLastFrame = true;
	else LB_DownInLastFrame = false;

	clear();

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (LB_DownInLastFrame) m_events[GameEvent_LeftButtonStay] = true;
		else m_events[GameEvent_LeftButtonDown] = true;
	}
}