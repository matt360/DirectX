#pragma once
#include "../DXFramework/D3D.h"
#include "../DXFramework/BaseMesh.h"
#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Camera.h"
#include "../DXFramework/TextureManager.h"

enum class MESH { 
	TRIANGLE,
	SPHERE,
	CUBE,
	QUAD,
	PLANE 
};

class Example
{
public:
	Example() {}
	~Example() {}

	virtual void initShader(D3D * renderer, HWND hwnd) = 0;
	
	float over_time;
	bool wireframe;
	bool example;
	bool triangle_mesh, sphere_mesh, cube_mesh, quad_mesh, plane_mesh;
	XMFLOAT3 scale;
};

