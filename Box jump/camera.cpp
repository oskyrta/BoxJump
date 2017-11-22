////////////////////////////////////////////////
// Include
#include "camera.h"
#include "settingsManager.h"

extern SettingsManager settingsManager;

////////////////////////////////////////////////
// Class Camera
Camera::Camera()
{
	m_position = Vec2(0, 0);

	m_size = Vec2(0, 0);

	m_pixelSize = 1;

	m_renderWindow = 0;

	m_view = sf::View();
}

Camera::~Camera()
{
	if (m_renderWindow)
		delete m_renderWindow;
}

sf::Color getBgBolor(std::string& name)
{
	sf::Color bg;
	bg.r = settingsManager.p_windowSettings->get<int>(name + ".bgColor.r");
	bg.g = settingsManager.p_windowSettings->get<int>(name + ".bgColor.g");
	bg.b = settingsManager.p_windowSettings->get<int>(name + ".bgColor.b");

	return bg;
}

void Camera::createWindow(std::string camera)
{
	Vec2 size;

	size.x = settingsManager.p_windowSettings->get<float>(camera+".size.x");
	size.y = settingsManager.p_windowSettings->get<float>(camera + ".size.y");

	std::string name = settingsManager.p_windowSettings->get<std::string>(camera + ".name");

	int pixelSize = settingsManager.p_windowSettings->get<int>(camera + ".pixelSize");

	m_renderWindow = new sf::RenderWindow(
		sf::VideoMode(size.x, size.y, 32),
		name,
		sf::Style::Titlebar | sf::Style::Close
	);

	m_size = size;
	m_view = sf::View( sf::FloatRect(0, 0, (int)size.x, (int)size.y) );
	m_view.setCenter( sf::Vector2f(size.x / 2, size.y / 2) );
	m_position = size / 2;
	m_pixelSize = pixelSize;

	setPosition(Vec2());

	m_backgoundColor = getBgBolor(camera);
}

bool Camera::cameraFrame()
{
	if (!m_renderWindow->isOpen())
		return false;

	sf::Event event;
	while (m_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_renderWindow->close();
	}

	return true;
}

void Camera::clearWindow()
{
	m_renderWindow->clear(m_backgoundColor);
}

void Camera::setPosition(Vec2 position)
{
	m_view.move( sf::Vector2f(position.x - m_position.x, position.y - m_position.y) );
	m_renderWindow->setView(m_view);
	m_position = position;
}