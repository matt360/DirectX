// Main.cpp
#include "../DXFramework/System.h"
#include "RenderToTextureExample.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	RenderToTextureExample* app = new RenderToTextureExample();
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