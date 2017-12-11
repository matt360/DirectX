// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "TessellationShader.h"
#include "LightShader.h"
#include "TerrainShader.h"

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
	void loadTextures();
	
	// SPECULAR LIGHT ////////////////////
	bool specular_light_wireframe;
	// shader handler
	LightShader* lightShader;
	// mesh
	// light
	void initLight();
	Light* light;
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
	bool tessellation_shader;
	void renderTessellationExample();
	//////////////////////////////////////

	// TERRAIN ///////////////////////////
	bool terrain_wireframe;
	TerrainShader* terrainShader;
	// light
	Light* light_terrain;
	float light_terrain_y;
	// render function
	bool terrain_example;
	void renderTerrainExample();
	//////////////////////////////////////

	////////////// ImGUI /////////////////
	void initGui();

	ImVec4 clear_col;
	//////////////////////////////////////

	// math
	float clamp(float n, float lower, float upper);
};

#endif
