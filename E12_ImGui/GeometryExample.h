#pragma once
#include "Example.h"
#include "GeometryShader.h"

class GeometryExample : public Example
{
public:
	GeometryExample();
	~GeometryExample();

	void init(D3D* renderer, HWND hwnd);
	void initShader(D3D* renderer, HWND hwnd);
	//void initLight();
	void render(D3D* renderer, Camera* camera, TextureManager* textureMgr);

	// shader handler
	MESH mesh_choice;
	GeometryShader* shader;
	BaseMesh* mesh;
	bool triangle_mesh, sphere_mesh, cube_mesh, quad_mesh, plane_mesh;
	bool d3d11_primitive_topology_trianglelist, d3d11_primitive_topology_pointlist;
};

