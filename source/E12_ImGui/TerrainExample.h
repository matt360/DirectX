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

private:
	// shader handler
	TerrainShader* shader_;
	// light
	Light* light_;

	string height_texture_, mapping_texture_1_, mapping_texture_2_;

	float frequency_, choice_;
};

