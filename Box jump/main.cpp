#ifndef _DEBUG
#	pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG

////////////////////////////////////////////////
// Include
#include "game.h"
#include "eventController.h"
#include "interface.h"
#include "camera.h"
#include <iostream>
#include "settingsManager.h"

////////////////////////////////////////////////
// Global
SettingsManager settingsManager;

////////////////////////////////////////////////
// Main
int main()
{
	Game game;
	Interface gameInterface;
	EventController eventController;
	Camera camera;

	settingsManager.initialize();

	// Initialize camera
	camera.createWindow("MainCamera");

	// Setup game
	gameInterface.setupInterface();
	game.setupSystem();
	std::cout << "System setup complited\n";

	// Send pointers
	gameInterface.setEventController(&eventController);
	gameInterface.setGame(&game);
	game.setEventController(&eventController);
	game.setCamera(&camera);

	// Initialize game
	gameInterface.initialize();
	game.initialize();
	std::cout << "Game initialize complited\n";


	while (true)
	{
		// Start frame
		camera.clearWindow();

		eventController.update();
		gameInterface.update();
		if (!game.frame()) break;
		gameInterface.render();

		// End frame
		camera.getRenderWindow()->display();
	}

	game.shutdown();

	return 0;
}