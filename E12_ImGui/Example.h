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
	Example();
	virtual ~Example();

	virtual void initShader(D3D * renderer, HWND hwnd) = 0;
	virtual void render(D3D* renderer, Camera* camera, TextureManager* textureMgr) = 0;
	// Mesh choice
	MESH_CHOICE mesh_choice_;
	inline MESH_CHOICE& getMeshChoice() { return mesh_choice_; }
	// Mesh
	BaseMesh* mesh;
	inline void setMesh(BaseMesh* set_mesh) { mesh = set_mesh; }
	void set_mesh_choice(const MESH_CHOICE msh_ch);
	//inline BaseMesh* getMesh() { return mesh; }
	//inline BaseMesh& getAddress() { return *mesh; }
	// used for moving vertices over time
	float over_time_;
	//inline float getOverTime() { return over_time_; }
	// toggle wireframe mode on/off
	bool wireframe_;
	//inline void setWireframe(bool status) { wireframe_ = status; }
	//inline bool getWireframe() { return wireframe_; }
	// toggle example on/off
	bool example_;
	//inline void setExample(bool status) { example = status; }
	//inline bool getExample() { return example; }
	// toggle geometry meshes on/off
	bool triangle_mesh_, sphere_mesh_, cube_mesh_, quad_mesh_, plane_mesh_;
	// toggle primitive topology list
	bool d3d11_primitive_topology_trianglelist_, d3d11_primitive_topology_pointlist_;
	// scale_ mesh in example
	XMFLOAT3 scale_;
};

