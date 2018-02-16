#pragma once
////////////////////////////////////////////////
// Include
#include <string>
#include <list>
#include <SFML\Graphics.hpp>
#include "tools/vec2.h"
#include <boost\property_tree\ptree.hpp>

////////////////////////////////////////////////
// Forvard declaration
class Camera;
class UIObject;
class EventController;
class DataManager;

typedef boost::property_tree::ptree PropertyTree;

////////////////////////////////////////////////
// Class InterfaceWindow
class InterfaceWindow
{
public:
	InterfaceWindow();
	~InterfaceWindow();

	void initialize(std::string name);

	void createButtons();
	void createTexts();
	void createSprites();

	void update();
	void render();

	void setActive(bool active) { m_isActive = active; };
	bool getActive() { return m_isActive; };

	void setCamera(Camera* camera) { m_camera = camera; };

	UIObject* getObject(std::string name);

private:
	std::list<UIObject*> m_objectsList;
	bool m_isActive;

	std::string m_name;

	PropertyTree *m_objects;

	Camera* m_camera;
	EventController* m_eventController;
	DataManager* m_dataManager;
};