// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "App9.h"

class GraphicsApp : public BaseApplication
{
public:

	GraphicsApp();
	~GraphicsApp();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

	bool render();
protected:
	void gui();

private:
	ColourShader* colourShader;
	TriangleMesh* mesh;
	ImVec4 clear_col;
	bool isWireframe;

	void triangleColourShader();
	void trianglegui();
	bool isTriangleColourShader;
};

#endif

