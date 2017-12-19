#pragma once
#include "../DXFramework/DXF.h"
#include "MultiLightShader.h"
#include "GeometryShader.h"
#include "SpecularLightExample.h"
#include "TerrainExample.h"
#include "TessellationExample.h"
#include "MultiLightExample.h"

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
	MultiLightExample multiLightExample;
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
