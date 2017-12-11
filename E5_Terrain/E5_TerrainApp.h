// Application.h
#ifndef _E5_TERRAINAPP_H
#define _E5_TERRAINAPP_H

// Includes
#include "../DXFramework/DXF.h"
#include "LightShader.h"

class E5_TerrainApp : public BaseApplication
{
public:

	E5_TerrainApp();
	~E5_TerrainApp();
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
	LightShader* lightShader;
	
	Light* m_Light;

	float light_y;

	bool wireframe;
};

#endif