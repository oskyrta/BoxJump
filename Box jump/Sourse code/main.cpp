#ifndef _DEBUG
#	pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG

////////////////////////////////////////////////
// Include
#include "game.h"
#include "eventSystem\eventController.h"
#include "interface\interface.h"
#include "dataManager\dataManager.h"
#include "render\render.h"
#include "inputController.h"
#include <iostream>

Game* game;
Interface* gameInterface;
EventController* eventController;
DataManager* dataManager;
Render* renderSystem;
InputController* inputController;

void setup() {
	game = Game::instance();
	gameInterface = Interface::instance();
	eventController = EventController::instance();
	dataManager = DataManager::instance();
	inputController = InputController::instance();
	renderSystem = Render::instance();

	dataManager->loadData();
	inputController->setup();

	renderSystem->setup();

	gameInterface->setupInterface();
	game->setupSystem();

	std::cout << "System setup complited\n";
}

void initialize() {
	gameInterface->initialize();
	game->initialize(GameMode_OnePlayer);
	std::cout << "Game initialize complited\n";
}

bool loop(){
	if (!renderSystem->frame())
		return false;
	renderSystem->clear();

	eventController->update();
	inputController->update();
	gameInterface->update();
	if(!game->frame()) return false;

	gameInterface->render();
	renderSystem->draw();

	return true;
}

void shutdown() {
	game->shutdown();

	delete game;
	delete gameInterface;
	delete eventController;
	delete dataManager;
	delete renderSystem;
	delete inputController;
}

////////////////////////////////////////////////
// Main
int main()
{
	setup();
	initialize();
	while (loop());
	shutdown();

	return 0;
}