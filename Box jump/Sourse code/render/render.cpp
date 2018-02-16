////////////////////////////////
// Include
#include "render/render.h"
#include "render/camera.h"

CameraType GetCameraTypeByName(std::string name)
{
	if (name == "Main")
		return CameraType_Main;
}

////////////////////////////////
// Class Render

Render::~Render()
{
	for(int i = 0; i < CameraType_Count; i++)
		delete m_cameras[i];
}

Camera* Render::getCamera(std::string name)
{
	return m_cameras[GetCameraTypeByName(name)];
}

void Render::setup(int width, int height, std::string name, int scale, int framerate)
{
	m_renderWindow = new sf::RenderWindow(
		sf::VideoMode(width, height, 32),
		name,
		sf::Style::Titlebar | sf::Style::Close
	);

	m_scale = scale;

	if (framerate) {
		m_renderWindow->setFramerateLimit(framerate);
	}

	for (int i = 0; i < CameraType_Count; i++) {
		m_cameras[i] = 0;
	}
}

void Render::setupCamera(CameraType type, int width, int height, sf::Color color) {
	m_cameras[type] = new Camera(width, height, color);
	m_cameras[type]->setScale(m_scale);
}

bool Render::frame()
{
	// End if window is closed
	if (!m_renderWindow->isOpen())
		return false;

	// Check events
	sf::Event event;
	while (m_renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_renderWindow->close();
	}

	return true;
}

void Render::draw()
{
	//m_renderWindow->clear();

	// Draw image from cameras
	for (int i = 0; i < CameraType_Count; i++)
	{
		if (m_cameras[i])
			m_renderWindow->draw(m_cameras[i]->getSprite());
	}

	m_renderWindow->display();
}

void Render::clear()
{
	for (int i = 0; i < CameraType_Count; i++)
	{
		if (m_cameras[i])
			m_cameras[i]->clearTexture();
	}
}