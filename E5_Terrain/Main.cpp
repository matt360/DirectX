// Main.cpp
#include "../DXFramework/System.h"
#include "E5_TerrainApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	E5_TerrainApp* app = new E5_TerrainApp();
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