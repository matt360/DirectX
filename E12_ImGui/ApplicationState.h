#pragma once

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "GraphicsApp.h"

class ApplicationState : public BaseApplication
{
public:
	ApplicationState();
	~ApplicationState();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

protected:
	bool render();
	void gui();

private:
	BaseApplication* graphicsApp;
	/*ColourShader* colourShader;
	TriangleMesh* mesh;
	ImVec4 clear_col;
	bool isWireframe;

	void triangleColourShader();
	void trianglegui();
	bool isTriangleColourShader;*/
};

