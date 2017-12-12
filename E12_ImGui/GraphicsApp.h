// Application.h
#pragma once

// Includes
#include "../DXFramework/DXF.h"
#include "TessellationShader.h"
#include "SpecularLightShader.h"
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
	void initGuiVariables();
	
	// SPECULAR LIGHT ////////////////////
	bool specular_light_wireframe;
	// shader handler
	SpecularLightShader* specularLightShader;
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
	XMFLOAT3 tr_scale;
	//////////////////////////////////////

	// MULTI LIGHTS //////////////////////
	bool ml_wireframe;
	bool ml_triangle_mesh, ml_sphere_mesh, ml_cube_mesh, ml_quad_mesh, ml_plane_mesh;
	MultiLightShader* multiLightShader;

	Light *light0_, *light1_, *light2_, *light3_;
	ImVec4 light0_col, light1_col, light2_col, light3_col;
	XMFLOAT3 light0_pos, light1_pos, light2_pos, light3_pos;
	XMFLOAT3 ml_scale;
	//SphereMesh *lightSphere0_, *lightSphere1_, *lightSphere2_, *lightSphere3_;
	float light_y;
	// render function
	bool multi_light_example;
	void renderMultiLightExample();
	//////////////////////////////////////

	// math
	float clamp(float n, float lower, float upper);

	// extra functions
	void resetCamera();
	void resetCamera(float x, float y, float z, float pitch, float yaw, float roll);
};
