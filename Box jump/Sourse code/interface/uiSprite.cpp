////////////////////////////////////////////////
// Include
#include "uiSprite.h"
#include "render/camera.h"
#include "dataManager\dataManager.h"


////////////////////////////////////////////////
// Class UISprite
UISprite::UISprite()
{
	// Initialize sprite
	m_spriteSize = 1;
	m_spriteContainer = new SpriteContainer();
}

UISprite::~UISprite()
{
	if (m_sprite)
		delete m_sprite;
}

void UISprite::initialize(PropertyTree* tree) {
	UIObject::initialize(tree);

	m_spriteName = m_tree->get<std::string>("SpriteName");
	resetSprite(m_spriteName);
}

void UISprite::resetSprite(std::string name) {
	m_tree->put("SpriteName", name);
	m_spriteName = name;

	m_spriteSize = m_tree->get<int>("scale");

	m_spriteContainer = m_dataManager->getSpriteContainer(m_spriteName);
	m_sprite = new sf::Sprite();
	m_sprite->setTexture(*m_spriteContainer->texturePtr);
	m_sprite->setTextureRect(m_spriteContainer->rect);
	m_sprite->setScale(m_spriteSize, m_spriteSize);

	m_sprite->setOrigin(m_spriteContainer->rect.width / 2, m_spriteContainer->rect.height / 2);

	m_size.x = m_sprite->getGlobalBounds().width;
	m_size.y = m_sprite->getGlobalBounds().height;
}

void UISprite::render()
{
	if ( m_spriteContainer->rect.height != 0)
	{
		// Set position relative to the camera
		m_sprite->setPosition(roundf(m_pos.x) + m_camera->getCenterPosition().x, roundf(m_pos.y) + m_camera->getCenterPosition().y);

		m_camera->getRenderWindow()->draw(*m_sprite);
	}
	UIObject::render();
}

void UISprite::update()
{
	UIObject::update();
}