// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "DepthShader.h"
#include "ShadowShader.h"

class App10 : public BaseApplication
{
public:

	App10();
	~App10();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	void initLight();

	bool frame();

protected:
	bool render();
	void gui();

private:
	TerrainMesh* terrainMesh;
	DepthShader* depthShader;
	ShadowShader* shadowShader;

	Light* light;
};

#endif