//////////////////////////////////////////////
// Include
#include "uiObject.h"
#include <SFML\Graphics.hpp>
#include "render/camera.h"
#include "tools/utils.h"
#include "dataManager\dataManager.h"
#include "inputController.h"
#include <iostream>

//////////////////////////////////////////////
// Class GUIObject
UIObject::UIObject() {
	m_dataManager = DataManager::instance();
	m_inputController = InputController::instance();

	m_name = "";
	m_camera = 0;
}

UIObject::~UIObject() {
	m_relativePos = m_pos / m_camera->getHalfSize();
	m_tree->put("x", m_relativePos.x);
	m_tree->put("y", m_relativePos.y);
}

void UIObject::initialize(PropertyTree* tree) {
	m_tree = tree;

	m_relativePos.x = m_tree->get<float>("x");
	m_relativePos.y = m_tree->get<float>("y");

	m_pos = (m_relativePos * m_camera->getHalfSize()).getInt();
}

void UIObject::update() {
	checkMouseOnObject();
}

void UIObject::render() {
	//drawObjectPosition();
}

void UIObject::checkMouseOnObject() {
	m_mouseOnObject = false;

	if (m_inputController->mouseOnWindow()) {
		Vec2 mousePosition = m_inputController->getMousePosition() - m_camera->getHalfSize();

		// Check intersect with mouse
		if (abs(mousePosition.x - m_pos.x) <= m_size.x/2 && abs(mousePosition.y - m_pos.y) <= m_size.y/2)
			m_mouseOnObject = true;
	}
}

void UIObject::drawObjectPosition()
{
	// Draw position
	// /*
	Vec2 pos = m_pos + m_camera->getCenterPosition();
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(pos.x, pos.y - 5)),
		sf::Vertex(sf::Vector2f(pos.x, pos.y + 4))
	};
	m_camera->getRenderWindow()->draw(line, 2, sf::Lines);

	line[0] = sf::Vector2f(pos.x - 5, pos.y);
	line[1] = sf::Vector2f(pos.x + 4, pos.y);
	m_camera->getRenderWindow()->draw(line, 2, sf::Lines);
	// */
}