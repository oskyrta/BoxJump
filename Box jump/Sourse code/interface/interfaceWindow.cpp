////////////////////////////////////////////////
// Include
#include "interfaceWindow.h"

#include "render/camera.h"
#include "interface/uiButton.h"
#include "interface/uiText.h"
#include "interface/uiObject.h"
#include "interface/uiSprite.h"
#include "eventSystem/eventController.h"
#include "tools/utils.h"
#include "dataManager\dataManager.h"
#include "inputController.h"

#include <boost\foreach.hpp>

#include <string>

////////////////////////////////////////////////
// Extern

////////////////////////////////////////////////
// Class InterfaceWindow
InterfaceWindow::InterfaceWindow()
{
	m_isActive = false;
	m_dataManager = DataManager::instance();
	m_eventController = EventController::instance();
}

InterfaceWindow::~InterfaceWindow()
{
	// Delete interface objects
	for each (UIObject* obj in m_objectsList)
	{
		if (obj != 0)
		{
			delete obj;
			obj = 0;
		}
	}

	m_objectsList.clear();
}

void InterfaceWindow::createButtons()
{
	PropertyTree* buttons = m_objects->get_child_optional("Buttons").get_ptr();

	BOOST_FOREACH(auto &v, *buttons)
	{
		UIButton* button = new UIButton();
		button->setName(v.first);
		button->setCamera(m_camera);
		button->initialize(&v.second);

		m_objectsList.push_back(button);
	}
}

void InterfaceWindow::createTexts()
{
	PropertyTree *texts = m_objects->get_child_optional("Texts").get_ptr();

	BOOST_FOREACH(auto &v, *texts)
	{
		UIText* text = new UIText();
		text->setName(v.first);
		text->setCamera(m_camera);
		text->initialize(&v.second);

		m_objectsList.push_back(text);
	}
}

void InterfaceWindow::createSprites()
{
	PropertyTree *sprites = m_objects->get_child_optional("Images").get_ptr();

	if (sprites) {
		BOOST_FOREACH(auto &v, *sprites)
		{
			UISprite* sprite = new UISprite();
			sprite->setName(v.first);
			sprite->setCamera(m_camera);
			sprite->initialize(&v.second);

			m_objectsList.push_back(sprite);
		}
	}
}

void InterfaceWindow::initialize(std::string name)
{
	m_name = name;
	m_objects = m_dataManager->getInterfaceSettings()->get_child_optional(name).get_ptr();

	createSprites();
	createButtons();
	createTexts();
}


void InterfaceWindow::update()
{
	UIObject* movedObject = 0;
	for each (UIObject* obj in m_objectsList) {
		obj->update();

		if (obj->getMouseOnObject() && (movedObject == 0 || (movedObject != 0 && !movedObject->isDragged()) )) movedObject = obj;
	}

	if (movedObject) {
		if (IsKeyDown(0x70) && InputController::instance()->getBindState(Bind_LeftButton) & KeyState_Pressed) {
			movedObject->setIsDragged(true);
			movedObject->setPosition(InputController::instance()->getMousePositionC());
		}
		else {
			movedObject->setIsDragged(false);
		}
	}
}

void InterfaceWindow::render()
{
	for each (UIObject* obj in m_objectsList) {
		obj->render();
	}
}

UIObject* InterfaceWindow::getObject(std::string name)
{
	for each (UIObject* object in m_objectsList)
	{
		if (object->getName() == name)
			return object;
	}

	return 0;
}