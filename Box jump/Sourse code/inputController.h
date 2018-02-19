#pragma once
//////////////////////////////
// Include
#include <boost\property_tree\ptree.hpp>
#include <SFML\Graphics.hpp>
#include "tools\vec2.h"

//////////////////////////////
// Forward declaration
class DataManager;
class Render;
struct Bind;

enum KeyState
{
	KeyState_None = 0,
	KeyState_Down = 1 << 0,
	KeyState_Pressed = 1 << 1,
	KeyState_Up = 1 << 2
};

enum BindType
{
	Bind_None,
	Bind_LeftButton,
	Bind_RightButton,
	Bind_Enter,
	Bind_Esc,
	Bind_P1Left,
	Bind_P1Right,
	Bind_P1Jump,
	Bind_P2Left,
	Bind_P2Right,
	Bind_P2Jump,

	Bind_Count
};

BindType GetBindByName(const std::string & name);

//////////////////////////////
// Class InputController
class InputController
{
public:
	InputController() {};

	static InputController* instance()
	{
		static InputController *instance_ = new InputController();
		return instance_;
	};

	short getBindState(int bind);
	bool isKeyDown(int bind);
	bool isKeyUp(int bind);
	bool isKeyPressed(int bind);

	Vec2 getMousePosition();
	Vec2 getMousePositionC();
	bool mouseOnWindow();

	void setup();
	void update();

private:
	Render* m_render;
	DataManager* m_dataManager;
	boost::property_tree::ptree* m_tree;

	Vec2 m_mousePosition;
	Vec2 m_cameraTopLeft;

	int m_windowScale;

	Bind* m_binds[Bind_Count];
};