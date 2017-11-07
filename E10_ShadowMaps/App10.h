// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "DepthShader.h"
#include "ShadowShader.h"

const int SHADOWMAP_WIDTH = 1024;
const int SHADWOMAP_HEIGHT = 1024;

class App10 : public BaseApplication
{
public:

	App10();
	~App10();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();
protected:
	void initLight();
	void RenderToTexture();
	void RenderScene();

	bool render();
	void gui();

private:
	TerrainMesh* terrainMesh;
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;

	DepthShader* depthShader;
	ShadowShader* shadowShader;
	RenderTexture* renderTexture;

	Light* light;
};

#endif