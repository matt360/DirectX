// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "TessellationShader.h"
#include "LightShader.h"
#include "TerrainShader.h"
#include "MultiLightShader.h"

class GraphicsApp : public BaseApplication
{
public:

	GraphicsApp();
	~GraphicsApp();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

protected:
	bool render();
	void gui();

private:
	// CONSTRUCTOR
	void initVariables();
	void initShaders(HWND hwnd);
	void initGeometry();
	void loadTextures();
	void initLight();
	
	// SPECULAR LIGHT ////////////////////
	bool specular_light_wireframe;
	// shader handler
	LightShader* lightShader;
	// mesh
	// specular_light
	Light* specular_light;
	// render function
	bool specular_light_example;
	void renderSpecularLightExample();
	//////////////////////////////////////

	// TESSELLATION //////////////////////
	bool tessellation_wireframe;
	// shader handler
	TessellationShader* tessellationShader;
	// meshes
	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	TerrainMesh *terrainMesh;
	// render function
	bool tessellation_example;
	void renderTessellationExample();
	//////////////////////////////////////

	// TERRAIN ///////////////////////////
	bool terrain_wireframe;
	TerrainShader* terrainShader;
	// specular_light
	Light* light_terrain;
	float light_terrain_y;
	// render function
	bool terrain_example;
	void renderTerrainExample();
	//////////////////////////////////////

	// MULTI LIGHTS //////////////////////
	bool multi_light_wireframe;
	MultiLightShader* multiLightShader;

	Light *light0_, *light1_, *light2_, *light3_;
	//SphereMesh *lightSphere0_, *lightSphere1_, *lightSphere2_, *lightSphere3_;
	float light_y;
	// render function
	bool multi_light_example;
	void renderMultiLightExample();
	//////////////////////////////////////

	////////////// ImGUI /////////////////
	void initGuiVariables();

	ImVec4 clear_col;
	//////////////////////////////////////

	// math
	float clamp(float n, float lower, float upper);
};

#endif
