#pragma once
////////////////////////
// Include
#include "SFML\Graphics.hpp"

////////////////////////
// Enum
enum CameraType
{
	CameraType_Main,

	CameraType_Count
};

////////////////////////
// Forward declaration
class Camera;

////////////////////////
// Class Render
class Render
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

	void setup(int width, int height, std::string name, int scale = 1, int framerate = 0);
	void setupCamera(CameraType type, int width, int height, sf::Color color);
	bool frame();
	void draw();
	void clear();

	Camera* getCamera(CameraType type) { return m_cameras[type]; }
	Camera* getCamera(std::string);
	sf::RenderWindow* getRenderWindow() { return m_renderWindow; };
	sf::Vector2u getSize() { return m_renderWindow->getSize(); };
	int getScale() { return m_scale; }

private:
	Camera* m_cameras[CameraType_Count];

	int m_scale;

	sf::RenderWindow *m_renderWindow;
};