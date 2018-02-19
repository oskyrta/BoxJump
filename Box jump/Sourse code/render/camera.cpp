////////////////////////////////////////////////
// Include
#include "render/camera.h"
#include "tools\vec2.h"
#include <assert.h>

////////////////////////////////////////////////
// Class Camera
Camera::Camera()
{
	m_position = Vec2();
	m_size = Vec2();

	m_renderTexture = 0;

	m_view = sf::View();
}

void Camera::init(int width, int height, sf::Color color)
{
	if (!m_renderTexture) {
		m_renderTexture = new sf::RenderTexture();
	}
	else {
		m_renderTexture->clear();
	}
	assert( m_renderTexture->create(width, height) );

	m_size = Vec2(width, height);
	m_halfSize = m_size / 2;
	m_view = sf::View(sf::FloatRect(0, 0, width, height));
	m_position = Vec2();
	m_onScreenPosition = Vec2((float)width , (float)height);

	m_sprite.setPosition(m_onScreenPosition.getSFVector());
	m_sprite.setOrigin((float)width / 2, (float)height / 2);

	m_backgoundColor = color;
}

Camera::~Camera() {

}

sf::Sprite& Camera::getSprite() {
	m_renderTexture->display();
	m_sprite.setTexture(m_renderTexture->getTexture());

	return m_sprite;
}

void Camera::setPosition(Vec2 position) {
	m_realPosition = position;
	position.x = lround(position.x);
	position.y = lround(position.y);
	m_view.move(position.x - m_position.x, position.y - m_position.y);
	m_renderTexture->setView(m_view);
	m_position = position;
}