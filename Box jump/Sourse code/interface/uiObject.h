#pragma once
/////////////////////////////////////////////////
// Include
#include "tools/vec2.h"
#include <boost\property_tree\ptree.hpp>

/////////////////////////////////////////////////
// Forvard declaration
class Camera;
class DataManager;
class InputController;

typedef boost::property_tree::ptree PropertyTree;

/////////////////////////////////////////////////
// Class GUIObject
class UIObject
{
public:
	UIObject();
	virtual ~UIObject() ;

	virtual void render();
	virtual void update();

	virtual void initialize(PropertyTree* tree);

	void setPosition(Vec2 pos) { m_pos = pos; };
	void setPosition(float x, float y) { m_pos = Vec2(x, y); }
	Vec2 getPosition() { return m_pos; };

	void setSize(Vec2 size) { m_size = size; };
	void setSize(float width, float height) { m_size = Vec2(width, height); }
	Vec2 getSize() { return m_size; };

	void setCamera(Camera* camera) { m_camera = camera; };

	void setName(std::string name) { m_name = name; };
	std::string getName() { return m_name; };

	bool getMouseOnObject() { return m_mouseOnObject; };
	bool isDragged() { return m_isDragged; };
	void setIsDragged(bool dragged) { m_isDragged = dragged; };

protected:
	void checkMouseOnObject();
	void drawObjectPosition();

protected:
	PropertyTree *m_tree;
	bool m_mouseOnObject;
	bool m_isDragged;

	Vec2 m_pos;
	Vec2 m_relativePos;
	Vec2 m_size;

	std::string m_name;

	Camera* m_camera;
	DataManager* m_dataManager;
	InputController* m_inputController;
};