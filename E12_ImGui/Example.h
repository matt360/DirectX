#pragma once
#include "../DXFramework/D3D.h"
#include "../DXFramework/BaseMesh.h"
#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Camera.h"
#include "../DXFramework/TextureManager.h"

// current example mesh choice 
enum class MESH_CHOICE { 
	TRIANGLE,
	SPHERE,
	CUBE,
	QUAD,
	PLANE,
	TERRAIN
};

class Example
{
public:
	Example() {}
	virtual ~Example() {}

	virtual void initShader(D3D * renderer, HWND hwnd) = 0;
	virtual void render(D3D* renderer, Camera* camera, TextureManager* textureMgr) = 0;
	// Mesh choice
	MESH_CHOICE mesh_choice;
	// Mesh
	BaseMesh* mesh;
	inline void setMesh(BaseMesh* set_mesh) { mesh = set_mesh; }
	// used for moving vertices over time
	float over_time;
	// toggle wireframe mode on/off
	bool wireframe;
	// toggle example on/off
	bool example;
	// toggle geometry meshes on/off
	bool triangle_mesh, sphere_mesh, cube_mesh, quad_mesh, plane_mesh;
	// toggle primitive topology list
	bool d3d11_primitive_topology_trianglelist, d3d11_primitive_topology_pointlist;
	// scale mesh in example
	XMFLOAT3 scale;
};

