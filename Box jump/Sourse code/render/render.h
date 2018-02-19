#pragma once
////////////////////////
// Include
#include "SFML\Graphics.hpp"
#include "eventSystem\eventListener.h"
#include "tools\vec2.h"

////////////////////////
// Enum
enum CameraType {
	CameraType_Main,

	CameraType_Count
};

enum Resolutions {
	Resolution_None,
	Resolution_320x200,
	Resolution_640x400,
	Resolution_960x600,
	Resolution_1280x800,

	Resolution_Count
};

void getResolution(Resolutions resolution, int & width, int & height);
////////////////////////
// Forward declaration
class Camera;
class DataManager;
class EventController;

////////////////////////
// Class Render
class Render : EventListener
{
public:
	Render() {};
	~Render();

	// Single initialization
	static Render* instance()
	{
		static Render *instance_ = new Render();
		return instance_;
	};

	void setup();
	void initializeWindow();
	void setupCamera(CameraType type);
	bool frame();
	void draw();
	void clear();

	Camera* getCamera(CameraType type) { return m_cameras[type]; }
	Camera* getCamera(std::string);
	sf::RenderWindow* getRenderWindow() { return m_renderWindow; };
	sf::Vector2u getSize() { return m_renderWindow->getSize(); };
	int getScale() { return m_resolution; }
	Vec2 getTopLeftCameraPos(CameraType type);

	void chageResolution();

private:
	void startListeningEvents();

private:
	Camera* m_cameras[CameraType_Count];

	Resolutions m_resolution;

	sf::RenderWindow * m_renderWindow; 
	DataManager * m_dataManager;
	EventController * m_eventController;
};