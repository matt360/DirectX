// Main.cpp
// The C++ compiler is responisble for taking a text file and converting it into an intermediate format called an object file.
// In going from the text file (our C++ code) to the .exe file there 2 major operations that need to happend:
// - Compiling: 
// 	compiler needs to pre-process out code (any pre-process statement get evaluated). 
// 	Turing text (parsing, tokeninzing) into a forat that the complier can actually reson with. 
// 	This resulst in a syntax tree being created (Represenation of our code as an abstract syntax tree)
// The compilers job at the end of the data is to convert our code into either: constant data or intrusctions.
// - Linking


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
