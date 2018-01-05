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
	void initVariables();
	void initLight();
	void render(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	void gui(Camera* camera);
	void resetExample(Camera* camera);

	// shader handler
	TerrainShader* shader;
	// light
	Light* light;
};

