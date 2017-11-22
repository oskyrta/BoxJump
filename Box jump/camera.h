#pragma once
/////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>
#include "vec2.h"

/////////////////////////////////////////////////
// Class Camera
class Camera
{
public:
	Camera();
	~Camera();

	void setPosition(Vec2 position);
	Vec2 getPosition() { return m_position; };

	void setBackgroundColor(sf::Color color) { m_backgoundColor = color; };
	sf::Color getBackgoundColor() { return m_backgoundColor; }
	
	void setPixelSize(int size) { m_pixelSize = size; };
	int getPixelSize() { return m_pixelSize; };

	Vec2 getSize() { return m_size; };

	sf::RenderWindow* getRenderWindow() { return m_renderWindow; };

	void createWindow(std::string camera);
	void clearWindow();

	bool cameraFrame();

private:
	int m_pixelSize;

	Vec2 m_position;
	Vec2 m_size;

	sf::Color m_backgoundColor;
	sf::View m_view;
	sf::RenderWindow* m_renderWindow;
};