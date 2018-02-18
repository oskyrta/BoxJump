/////////////////////////////////////////////////
// Include
#include "uiButton.h"
#include "eventSystem\eventController.h"
#include "tools\utils.h"
#include "render/camera.h"
#include "dataManager\dataManager.h"
#include "inputController.h"

#include <iostream>

/////////////////////////////////////////////////
// Variable 
bool leftClick = false;

/////////////////////////////////////////////////
// Class UIButton
UIButton::UIButton()
{
	m_eventController = EventController::instance();
}

UIButton::~UIButton()
{
	int a = 0;
}


void UIButton::initialize(PropertyTree* tree)
{
	UIObject::initialize(tree);

	m_size.x = m_tree->get<int>("width");
	m_size.y = m_tree->get<int>("height");

	m_event = m_tree->get<std::string>("Event");
	m_bind = GetBindByName(m_tree->get<std::string>("Bind", ""));

	m_string = m_tree->get<std::string>("String", "");
	if (m_string != "")
	{
		m_characterSize = m_tree->get<int>("CharacterSize");
		m_color.r = m_tree->get<int>("Color.r");
		m_color.g = m_tree->get<int>("Color.g");
		m_color.b = m_tree->get<int>("Color.b");
		initializeText();
		m_textOffset = m_size * Vec2(m_align.x + 0.5, 0) * -1;
	}

	m_spriteName = m_tree->get<std::string>("SpriteName", "");
	if (m_spriteName != "") initializeSprite();
}

void UIButton::initializeSprite()
{
	m_spriteSize = m_tree->get<int>("scale", 1);

	m_spriteContainer = m_dataManager->getSpriteContainer(m_spriteName);
	m_sprite = new sf::Sprite();
	m_sprite->setTexture(*m_spriteContainer->texturePtr);
	m_sprite->setTextureRect(m_spriteContainer->rect);
	m_sprite->setScale(m_spriteSize, m_spriteSize);

	m_rect = m_spriteContainer->rect;

	m_sprite->setOrigin(m_spriteContainer->rect.width / 2, m_spriteContainer->rect.height / 2);

}

void UIButton::update() { 
	UIText::update();

	if (m_mouseOnObject) {
		if (m_offset.x < 4) {
			m_offset.x += .22f;
		}
		else {
			m_offset.x = 4;
		}
	}
	else if (m_offset.x > 0) {
		m_offset.x -= .22f;
	}


	if ((m_mouseOnObject && !IsKeyDown(0x70) && InputController::instance()->getBindState(Bind_LeftButton) & KeyState_Down) ||
		InputController::instance()->getBindState((BindType)m_bind) & KeyState_Down)
	{
		m_eventController->startEvent(m_event);
	}
}

void UIButton::render() {
	// Sprite render
	if (m_rect.height != 0){
		// Set position relative to the camera
		m_sprite->setPosition(roundf(m_pos.x) + m_camera->getCenterPosition().x, roundf(m_pos.y) + m_camera->getCenterPosition().y);

		m_camera->getRenderWindow()->draw(*m_sprite);
	}

	UIText::render();

	// Draw a border
	/*
	Vec2 pos = m_camera->getCenterPosition() + m_pos;
	sf::RectangleShape border(m_size.getSFVector());
	border.setOrigin((m_size / 2).getSFVector());
	border.setPosition(pos.getSFVector());
	border.setFillColor(sf::Color(0, 0, 0, 0));
	border.setOutlineThickness(1);
	border.setOutlineColor(sf::Color(20, 20, 20));

	m_camera->getRenderWindow()->draw(border);
	// */
}