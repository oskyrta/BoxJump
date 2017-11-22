#pragma once
/////////////////////////////////////////////////
// Include
#include "vec2.h"
#include <iostream>

/////////////////////////////////////////////////
// Forvard declaration
class Camera;

/////////////////////////////////////////////////
// Class GUIObject
class GUIObject
{
public:
	//GUIObject();
	~GUIObject();

	virtual void render() {};
	virtual void update();

	void setPosition(Vec2 pos) { m_pos = pos; };
	void setHalfSize(Vec2 halfSize) { m_halfSize = halfSize; };
	void setCamera(Camera* camera) { m_camera = camera; };
	void setTag(std::string tag) { m_tag = tag; };

	Vec2 getHalfSize() { return m_halfSize; };
	bool getMouseOnObject() { return m_mouseOnObject; };

protected:
	bool m_mouseOnObject;

	Vec2 m_pos;
	Vec2 m_halfSize;

	std::string m_tag;

	Camera* m_camera;
};