#pragma once

#include "Example.h"
#include "TerrainShader.h"

class TerrainExample : public Example
{
public:
	TerrainExample();
	~TerrainExample();

	void init(D3D * renderer, HWND hwnd);
	void initShader(D3D * renderer, HWND hwnd);
	void initLight();

	// shader handler
	TerrainShader* shader;
	// specular_light
	Light* light;
	float over_time;
	// render
	void render(D3D* renderer, Camera* camera, BaseMesh* mesh, TextureManager* textureMgr);
	XMFLOAT3 scale;
};

