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
	CubeMesh* cubeMesh;
	QuadMesh* quadMesh;
	PlaneMesh* planeMesh;
	TerrainMesh* terrainMesh;
	// Shader handler
	ColourShader* colourShader;
	LightShader* lightShader;
	
	Light* m_Light;

	float light_y;

	bool wireframe;
};

#endif