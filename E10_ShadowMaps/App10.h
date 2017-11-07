// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"

class App10 : public BaseApplication
{
public:

	App10();
	~App10();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void gui();

private:
	ColourShader* colourShader;
	TriangleMesh* mesh;

	
};

#endif