// GraphicsApp.h
#pragma once

// Includes
#include "../DXFramework/DXF.h"
#include "MultiLightShader.h"
#include "GeometryShader.h"
#include "SpecularLightExample.h"
#include "TerrainExample.h"
#include "TessellationExample.h"

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
	//void initShaders(HWND hwnd);
	void initGeometry();
	void loadTextures();
	void initLight();
	void initShaders(D3D* renderer, HWND hwnd);
	void initGuiVariables();
	// meshes
	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	TerrainMesh *terrainMesh;

	// SPECULAR LIGHT EXAMPLE ////////////////////
	SpecularLightExample specularLightExample;
	//////////////////////////////////////////////

	// TESSELLATION //////////////////////
	TessellationExample tessellationExample;
	//////////////////////////////////////

	// TERRAIN ///////////////////////////
	TerrainExample terrainExample;
	//////////////////////////////////////

	// MULTI LIGHTS //////////////////////
	// shader handler
	MultiLightShader* multiLightShader;
	bool ml_wireframe;
	bool ml_triangle_mesh, ml_sphere_mesh, ml_cube_mesh, ml_quad_mesh, ml_plane_mesh;

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

	// GEOMATRY SHADER ///////////////////
	// shader handler
	GeometryShader* geometryShader;
	bool gs_wireframe;
	bool gs_triangle_mesh, gs_sphere_mesh, gs_cube_mesh, gs_quad_mesh, gs_plane_mesh;
	bool d3d11_primitive_topology_trianglelist, d3d11_primitive_topology_pointlist;
	// render
	bool geometry_shader_example;
	void renderGeometryShaderExample();
	XMFLOAT3 gs_scale;
	//////////////////////////////////////
	// math
	float clamp(float n, float lower, float upper);
};
