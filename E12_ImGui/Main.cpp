// Main.cpp

/*
Ideally the coursework
should demonstrate the use of all shader stages:
- Vertex
- Pixel
- Tessellation (including Hull and Domain)
- Geometry
The coursework should also demonstrate key graphical techniques such as:
- Vertex manipulation
- Post processing
- Lighting and shadows
- Tessellation
- Geometry manipulation and/or generation
*/

#include "../DXFramework/System.h"
#include "App1.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	GraphicsApp* app = new GraphicsApp();
	System* m_System;

	// Create the system object.
	m_System = new System(app);

	// Initialize and run the system object.
	m_System->run();

	// Shutdown and release the system object.
	delete m_System;
	m_System = 0;

	return 0;
}