#pragma once
/////////////////////////////////////////////////
// Include
#include "vec2.h"
#include "eventListener.h"
#include <iostream>

/////////////////////////////////////////////////
// Forvard declaration
class Camera;

/////////////////////////////////////////////////
// Class GUIObject
class GUIObject
{
public:
	GUIObject();
	GUIObject(Camera* camera, std::string tag, Vec2 halfSize);
	virtual ~GUIObject() ;

	virtual void render() {};
	virtual void update();

	virtual void initialize() {};

	void setPosition(Vec2 pos) { m_pos = pos; };
	void setPosition(float x, float y) { m_pos = Vec2(x, y); }
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