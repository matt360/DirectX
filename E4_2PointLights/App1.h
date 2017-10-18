// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "LightShader.h"

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

	void initLight();
private:
	TriangleMesh* triangleMesh;
	SphereMesh* sphereMesh;
	QuadMesh* quadMesh;
	ColourShader* colourShader;
	LightShader* lightShader;
	
	Light *light0_, *light1_, *light2_, *light3_;

	float light_y;
};

#endif