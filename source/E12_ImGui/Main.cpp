// Main.cpp

#include "../DXFramework/System.h"
#include "GraphicsApp.h"

// main function in an entry point, an entry point for our application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	BaseApplication* app = new GraphicsApp();

	System* m_System;
	// Create the system object.
	m_System = new System(app);

	// Initialize and run the system object.
	m_System->run();

	// Shutdown and release the system object.
	delete m_System;
	m_System = 0;

	// if the main function doesn't return anything it will be assumed that it returned zero
	// return 0
}
