////////////////////////////////
// Include
#include "render/render.h"
#include "render/camera.h"
#include "dataManager\dataManager.h"
#include "inputController.h"


CameraType GetCameraTypeByName(std::string name)
{
	if (name == "Main")
		return CameraType_Main;
}
std::string GetCameraNameByType(CameraType type)
{
	switch (type) {
		case CameraType_Main: return "Main";
	}

	return "";
}
void getResolution(Resolutions resolution, int & width, int & height) {
	width = 0; height = 0;

	switch (resolution)
	{
	case Resolution_320x200:
		width = 320; height = 200;
		break;
	case Resolution_640x400:
		width = 640; height = 400;
		break;
	case Resolution_960x600:
		width = 960; height = 600;
		break;
	case Resolution_1280x800:
		width = 1280; height = 800;
		break;
	}
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

void Render::startListeningEvents() {
	m_eventController->addListenerToEvent(
		this,
		"OnChangeRosolutionButtonDown",
		[](const EventListener* listener) { ((Render*)(listener))->chageResolution(); }
	);
}

void Render::chageResolution() {
	m_resolution = (Resolutions)((m_resolution + 1) % Resolution_Count);
	if (m_resolution == 0) m_resolution = Resolution_320x200;

	int width, height;
	getResolution(m_resolution, width, height);

	m_dataManager->getWindowSettings()->put("width", width);
	m_dataManager->getWindowSettings()->put("height", height);
	m_dataManager->getWindowSettings()->put("scale", m_resolution);

	initializeWindow();
}

void Render::setup() {
	m_dataManager = DataManager::instance();
	m_eventController = EventController::instance();
	m_inputController = InputController::instance();

	startListeningEvents();

	initializeWindow();
}

void Render::initializeWindow() {
	int width = m_dataManager->getWindowSettings()->get<int>("width");
	int height = m_dataManager->getWindowSettings()->get<int>("height");
	std::string name = m_dataManager->getWindowSettings()->get<std::string>("name");
	Resolutions resolution = (Resolutions)m_dataManager->getWindowSettings()->get<int>("scale");
	int framerate = m_dataManager->getWindowSettings()->get<int>("Framerate", 0);

	if (m_renderWindow) {
		m_renderWindow->close();
		m_renderWindow = 0;
	}

	m_renderWindow = new sf::RenderWindow(
		sf::VideoMode(width, height, 32),
		name,
		sf::Style::Titlebar | sf::Style::Close
	);

	m_resolution = resolution;

	if (framerate) {
		m_renderWindow->setFramerateLimit(framerate);
	}

	for (int i = 0; i < CameraType_Count; i++) {
		setupCamera((CameraType)i);
	}
}

void Render::setupCamera(CameraType type) {
	std::string name = GetCameraNameByType(type);

	int width = m_dataManager->getWindowSettings()->get<int>(name + ".width");
	int height = m_dataManager->getWindowSettings()->get<int>(name + ".height");
	float x = m_dataManager->getWindowSettings()->get<float>(name + ".x");
	float y = m_dataManager->getWindowSettings()->get<float>(name + ".y");

	sf::Vector2u pos = m_renderWindow->getSize();
	pos.x *= x; pos.y *= y;

	sf::Color color;
	color.r = m_dataManager->getWindowSettings()->get<int>(name + ".bgColor.r");
	color.g = m_dataManager->getWindowSettings()->get<int>(name + ".bgColor.g");
	color.b = m_dataManager->getWindowSettings()->get<int>(name + ".bgColor.b");

	if(m_cameras[type] == 0)
		m_cameras[type] = new Camera();
	m_cameras[type]->init(width, height, color);
	m_cameras[type]->setScale(m_resolution);
	m_cameras[type]->getSprite().setPosition((sf::Vector2f)pos);
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
		if (event.type == sf::Event::Closed) {
			m_renderWindow->close();
		}
		if (event.type == sf::Event::GainedFocus) {
			m_inputController->setWindowFocus(true);
		}

		if (event.type == sf::Event::LostFocus) {
			m_inputController->setWindowFocus(false);
		}
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

Vec2 Render::getTopLeftCameraPos(CameraType type) {
	sf::FloatRect rect = m_cameras[type]->getSpriteGlobalBounds();
	return Vec2(rect.left, rect.top);
}