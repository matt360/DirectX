// Application.h
/*
NOTES
In simple terms the main change is that the programmable vertex shader has been broken into three parts.
These three parts are the vertex shader, the hull shader, and the domain shader.

Tessellation occurs between the hull and the domain shader.

Instead of sending triangle lists to the video card we now send patch lists.
A patch is a primitive made up of control points.
For example if we send a single triangle to the video card it is 
considered a single patch with three control points.

*/

#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "TessellationShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void gui();

	bool wireframe;
private:
	ColourShader* colourShader;
	TessellationShader* tessellationShader;

	TriangleMesh* mesh;
};

#endif