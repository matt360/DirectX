// Main.cpp
#include "../DXFramework/System.h"
#include "App9.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	App9* app = new App9();
	System* system;

	// Create the system object.
	system = new System(app);

	// Initialize and run the system object.
	system->run();

	// Shutdown and release the system object.
	delete system;
	system = 0;

	return 0;
}