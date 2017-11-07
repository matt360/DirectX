// Main.cpp
#include "../DXFramework/System.h"
#include "App10.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	App10* app = new App10();
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