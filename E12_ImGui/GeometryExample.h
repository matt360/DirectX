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
	void initVariables();
	//void initLight();
	void render(D3D* renderer, Camera* camera, TextureManager* textureMgr);

	// shader handler
	MESH_CHOICE mesh_choice = MESH_CHOICE::CUBE;
	GeometryShader* shader;
	BaseMesh* mesh;
};

